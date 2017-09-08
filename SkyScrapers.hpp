/**@author hoxnox <hoxnox@gmail.com>
 * @date 20170830
 *
 * https://github.com/hoxnox/kata-skyscrapers*/

#include <vector>
#include <list>
#include <array>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <memory>
#include <chrono>

#include "SkyScrapersHelpers.hpp"

/**@brief calculate vision for given line*/
template<class FwIt>
inline uint8_t
vision(FwIt begin, FwIt end)
{
    uint8_t vision = 0;
    uint8_t last_height = 0;
    while (begin != end)
    {
        if (*begin > last_height)
        {
            ++vision;
            last_height = *begin;
        }
        ++begin;
    }
    return vision;
}

template<uint8_t N>
class SkyScrapers
{
public:
    SkyScrapers();

    int** Solve(const int* clues);
    int** GetLastMatrix() const { return (int**)storage_[matrix_]->data(); }

private:
    using Line = std::array<uint8_t, N>;
    using Matrix = std::vector<Line>;

    ///@brief get clue counterpart index
    static uint8_t ridx(uint8_t i) { return (N-1 - i%N) + N*(i/N) + (i<N*2 ? 1 : -1)*2*N; }

    std::array<uint8_t, N*4> clues_{};  // current task
    std::array<uint8_t, 2>   filled_{}; // count of filled cols and rows
    int                      matrix_{}; // current solved matrix index

    // pre-generated permutations and visions 0 -> {}, 1 -> {{{3, 2, 1}, 3}, {{3, 1, 2}, 2}}, ...
    typedef struct Permutation__
    {
        Permutation__(Line& line = {}, uint8_t rev_vision = 0)
            : line(line), rev_vision(rev_vision) {};
        Line line;
        uint8_t rev_vision;
    } Permutation;
    std::array<std::vector<Permutation>, N> permutations_{};

    ///@brief replace matrix line with given line by clue index if it's possible
    static Matrix fill(Matrix m, const uint8_t* iline, bool fwd, uint8_t fixed, bool is_vertical);
    ///@brief get all possible lines for the given clue and try to fill every template
    ///
    /// @param threshold - if the size of the result is greater then threshold - give up
    std::vector<Matrix> generate(int clue_idx, const std::vector<Matrix>& templates, size_t threshold);

    ///@brief replace matrix element with given element if it's possible
    static Matrix fill(Matrix matrix, uint8_t element, uint8_t i, uint8_t j);
    ///@brief For every possible clue try to fill given element for all templates
    std::vector<Matrix> generate(uint8_t i, uint8_t j,
                                 const std::vector<Matrix>& templates);

    ///@brief estimate how much for the given clue will be generated candidates
    size_t estimate_count(uint8_t idx) const;

    ///@breaf for clue N and 1 the elements are defined
    ///@return rows and cols fille
    void fill_simple_clues(Matrix& m);

    ///@brief for every candidate brute zero elements
    int** brute_windows(const std::vector<Matrix>& candidates);

    ///@brief get indices for non zero and non N clues without counterparts
    std::list<uint8_t> clue_indices() const;

    // storage for answers (4x4 task requires int** as output)
    std::vector<std::shared_ptr<std::array<int*, N>>> storage_;
    std::vector<std::shared_ptr<std::array<int, N>>> row_storage_;
};

template<uint8_t N>
int**
SkyScrapers<N>::Solve(const int* clues)
{
    filled_={0, 0};
    std::transform(clues, clues + N*4, clues_.data(), [](int v){ return (uint8_t)v; });

    Matrix matrix{N};
    fill_simple_clues(matrix);

    std::list<uint8_t> pending = clue_indices();
    std::vector<Matrix> candidates{matrix};
    while (!pending.empty())
    {
        pending.sort([this](const uint8_t& lhv, const uint8_t& rhv)
                     { return estimate_count(lhv) < estimate_count(rhv); });
        auto best_pending = pending.begin();
        std::vector<Matrix> best_candidates;
        size_t min_sz = std::numeric_limits<size_t>::max();
        for(auto cur = pending.begin(); cur != pending.end(); ++cur)
        {
            auto tmp = generate(*cur, candidates, min_sz);
            LOG(INFO) << "   check " << (int)*cur << "(" << (int)clues_[*cur] << "): " << tmp.size() << std::endl;
            if (tmp.size() < min_sz && tmp.size() > 0)
            {
                best_candidates = tmp;
                min_sz = tmp.size();
                best_pending = cur;
                if (min_sz <= candidates.size()*4)
                    break;
            }
        }
        candidates = best_candidates;
        LOG(INFO) << (int)*best_pending << "(" << (int)clues_[*best_pending] << "): " << candidates.size() << std::endl;
        ++filled_[*best_pending/N%2];
        pending.erase(best_pending);
    }

    // at that moment there are only few candidates with some zero elements, let's brute
    return brute_windows(candidates);
}

template<uint8_t N>
SkyScrapers<N>::SkyScrapers()
{
    if (N > 9)
        throw std::runtime_error("Designed for N<9.");

    // put zero element into storage as zero solution
    storage_.emplace_back(new std::array<int*, N>{});
    for (uint8_t i = 0; i < N; ++i)
    {
        row_storage_.emplace_back(new std::array<int, N>{});
        storage_.back()->at(i) = row_storage_.back()->data();
    }

    // generate all permutations of {1, 2, ... N}
    std::array<uint8_t, N> v{};
    for (uint8_t i = 0; i < N; ++i)
        v[i] = i + 1;
    while(std::next_permutation(v.begin(), v.end()))
    {
        uint8_t vis = vision(v.begin(), v.end());
        permutations_[vis].emplace_back(v, vision(v.rbegin(), v.rend()));
    }
}

template<uint8_t N>
inline typename SkyScrapers<N>::Matrix
SkyScrapers<N>::fill(Matrix m, const uint8_t* iline, bool fwd, uint8_t fixed, bool is_vertical)
{
    // make attempt to fill line, if it's not possible - return empty matrix
    for (uint8_t i = 0; i < N; ++i)
    {
        uint8_t& v = (is_vertical ? m[i][fixed] : m[fixed][i]);
        if (v != 0 && v != *iline)
            return {};
        v = *iline;
        if (fwd)
            ++iline;
        else
            --iline;
    }

    // check row and col for duplicates
    for (uint8_t i = 0; i < N; ++i)
    {
        if (!is_vertical && i == fixed)
            continue;
        for (uint8_t j = 0; j < N; ++j)
        {
            if (is_vertical && j == fixed)
                continue;
            if ((is_vertical && m[i][j] == m[i][fixed]) || (!is_vertical && m[i][j] == m[fixed][j]))
                return {};
        }
    }

    return m;
}

template<uint8_t N>
std::vector<typename SkyScrapers<N>::Matrix>
SkyScrapers<N>::generate(int i, const std::vector<SkyScrapers<N>::Matrix>& templates, size_t max)
{
    if (clues_[i] == 0)
        return templates;

    std::vector<Matrix> rs;
    if (max < std::numeric_limits<size_t>::max())
        rs.reserve(max);
    uint8_t counter = 0;
    uint8_t rclue = clues_[ridx(i)];

    uint8_t fixed = (i<2*N ? i%N : N-1-i%N);
    uint8_t direction = i/N;
    bool is_vertical = (direction%2==0);

    for (const auto& tpl : templates)
    {
        for (const auto& line: permutations_[clues_[i]])
        {
            if (rclue == 0 || line.rev_vision == rclue)
            {
                auto matrix = (direction==1 || direction==2) ?
                    fill(tpl, line.line.data() + N-1, false, fixed, is_vertical) :
                    fill(tpl, line.line.data(), true, fixed, is_vertical);
                if (!matrix.empty())
                {
                    rs.emplace_back(std::move(matrix));
                    ++counter;
                }
            }
            if (counter > max)
                break;
        }
        if (counter > max)
            break;
    }
    rs.shrink_to_fit();
    return rs;
}

template<uint8_t N>
typename SkyScrapers<N>::Matrix
SkyScrapers<N>::fill(SkyScrapers<N>::Matrix matrix, uint8_t n, uint8_t i, uint8_t j)
{
    if (matrix[i][j] == n)
        return matrix;

    if (matrix[i][j] == 0)
    {
        for (uint8_t k = 0; k < N; ++k)
            if (matrix[k][j] == n || matrix[i][k] == n)
                return {};
        matrix[i][j] = n;
        return matrix;
    }

    return {};
}

template<uint8_t N>
std::vector<typename SkyScrapers<N>::Matrix>
SkyScrapers<N>::generate(uint8_t i, uint8_t j, const std::vector<SkyScrapers<N>::Matrix>& templates)
{
    std::vector<Matrix> rs;
    for (const auto& tpl : templates)
    {
        for (uint8_t n = 1; n < N+1; ++n)
        {
            auto matrix = fill(tpl, n, i, j);
            if (!matrix.empty())
                rs.emplace_back(matrix);
        }
    }
    // LOG(INFO) << "generated (" << (int)i << ", " << (int)j << "): " << rs.size() << std::endl;
    return rs;
}

template<uint8_t N>
void
SkyScrapers<N>::fill_simple_clues(Matrix& matrix)
{
    // The line for clue = N is determined, we should fill it
    auto clueN = std::find(clues_.begin(), clues_.end(), N);
    while (clueN != clues_.end())
    {
        Line line;
        for (uint8_t i = 0; i < N; ++i)
            line[i] = i + 1;
        uint8_t idx = clueN - clues_.begin();
        uint8_t fixed = (idx<2*N ? idx%N : N-1-idx%N);
        uint8_t direction = idx/N;
        bool is_vertical = (direction%2==0);
        matrix = (direction == 1 || direction == 2) ?
              fill(matrix, line.data() + N-1, false, fixed, is_vertical) :
              fill(matrix, line.data(), true, fixed, is_vertical);        ++filled_[idx/N%2];
        clueN = std::find(clueN+1, clues_.end(), N);
    }

    // matrix cell for clue = 1 is determined, we should fill it too
    auto clue1_ = std::find(clues_.begin(), clues_.end(), 1);
    if (clue1_ != clues_.end())
    {
        auto clue1 = clue1_ - clues_.begin();
        if (clue1 < N)
            matrix[0][clue1] = N;
        if (clue1 > N && clue1 < 2*N)
            matrix[clue1%N][N-1] = N;
        if (clue1 > 2*N && clue1 < 3*N)
            matrix[N-1][N-1-clue1%N] = N;
        if (clue1 > 3*N && clue1 < 4*N)
            matrix[N-1-clue1%N][0] = N;
    }
}

template<uint8_t N>
int**
SkyScrapers<N>::brute_windows(const std::vector<Matrix>& candidates)
{
    if (candidates.size() < 1)
        return nullptr;

    // get possible matrices for zero-clues
    for (const auto candidate : candidates)
    {
        // LOG(INFO) << "Full solve for " << std::endl << candidate << std::endl;
        std::vector<Matrix> filled_candidates = {candidate};
        for (uint8_t i = 0; i < N; ++i)
            for (uint8_t j = 0; j < N; ++j)
                if (candidate[i][j] == 0)
                    filled_candidates = generate(i, j, filled_candidates);

        if (filled_candidates.size() > 0)
        {
            // store solution
            storage_.emplace_back(new std::array<int*, N>);
            std::array<int*, N>& solution = *storage_.back();
            for (uint8_t i = 0; i < N; ++i)
            {
                row_storage_.emplace_back(new std::array<int, N>);
                std::array<int, N>& row = *row_storage_.back();
                for (uint8_t j = 0; j < N; ++j)
                    row[j] = filled_candidates[0][i][j];
                solution[i] = row.data();
            }
            matrix_ = storage_.size() - 1;
            return (int**)(storage_[matrix_]->data());
        }
    }
}

template<uint8_t N>
size_t
SkyScrapers<N>::estimate_count(uint8_t idx) const
{
    size_t perms = permutations_[clues_[idx]].size();
    if (clues_[ridx(idx)] != 0)
    {
        size_t count = 0;
        for (const auto& p : permutations_[clues_[idx]])
            if (p.rev_vision == clues_[ridx(idx)])
                ++count;
        perms = count;
    }

    size_t redux = 1;
    for (uint8_t i = 0; i < filled_[(idx+1)/N%2]; ++i)
        redux *= N-i;

    return perms/redux;
}

template<uint8_t N>
std::list<uint8_t>
SkyScrapers<N>::clue_indices() const
{
    std::list<uint8_t> rs;
    for (uint8_t i = 0; i < clues_.size(); ++i)
    {
        if (clues_[i] != 0 && clues_[i] != N)
        {
            if (clues_[ridx(i)] != 0 && i >= N*2)
                continue;
            rs.emplace_back(i);
        }
    }
    return rs;
}

