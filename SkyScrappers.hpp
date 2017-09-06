/**@author hoxnox <hoxnox@gmail.com>
 * @date 20170830
 *
 * https://github.com/hoxnox/skyscrappers*/

#include <vector>
#include <list>
#include <array>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <memory>

#define INFO "INFO"
#define ERROR "ERROR"
#define LOG(X) std::cout << (X) << ": "

template<uint8_t N>
class SkyScrappers
{
public:
    SkyScrappers();

    int** Solve(const int* clues);
    int** GetLastMatrix() const { return (int**)storage_[matrix_].data(); }
    std::string PrintLastMatrix() const;

public:
    using Line = std::array<uint8_t, N>;
    using Matrix = std::vector<Line>;

    ///@brief get clue counterpart index
    static uint8_t ridx(uint8_t i) { return (N-1 - i%N) + N*(i/N) + (i<N*2 ? 1 : -1)*2*N; }

private:
    std::array<uint8_t, N*4> clues_{};
    int                      matrix_{};

    // pre-generated permutations and visions 0 -> {}, 1 -> {{{3, 2, 1}, 3}, {{3, 1, 2}, 2}}, ...
    typedef struct Permutation__
    {
        Permutation__(Line& line = {}, uint8_t rev_vision = 0)
            : line(line), rev_vision(rev_vision) {};
        Line line;
        uint8_t rev_vision;
    } Permutation;
    std::array<std::shared_ptr<std::vector<Permutation>>, N> permutations_{};

    ///@brief replace matrix line with given line by clue index if it's possible
    static Matrix fill(Matrix matrix, std::array<uint8_t, N> line, int idx);
    ///@brief get all possible lines for the given clue and try to fill for all templates
    std::vector<Matrix> generate(std::array<uint8_t, N*4> clues, int i,
                                 const std::vector<Matrix>& templates);
    ///@brief replace matrix element with given element if it's possible
    static Matrix fill(Matrix matrix, uint8_t n, uint8_t i, uint8_t j);
    ///@brief For every possible clue try to fill given element for all templates
    std::vector<Matrix> generate(uint8_t i, uint8_t j,
                                 const std::vector<Matrix>& templates);

    // storage for answers (task requires int** as output)
    std::vector<std::shared_ptr<std::array<int*, N>>> storage_;
    std::vector<std::shared_ptr<std::array<int, N>>> row_storage_;
};

template<uint8_t N>
std::string
SkyScrappers<N>::PrintLastMatrix() const
{
    auto clue_as_char =
        [](uint8_t clue) -> char
        {
            return ((clue > 0 && clue <= 9) ? clue + '0' : ' ');
        };

    auto print_clues =
        [&clue_as_char](std::ostream& s, std::array<uint8_t, N*4> clues, int start) -> void
        {
            s << "   ";
            if (start < N*2)
            {
                for (int i = start; i < start + N; ++i)
                    s << clue_as_char(clues[i]) << ' ';
            }
            else
            {
                for (int i = start + N - 1; i >= start; --i)
                    s << clue_as_char(clues[i]) << ' ';
            }
            s << "  \n";
        };

    auto print_row =
        [&clue_as_char](std::ostream& s,
                        uint8_t lclue,
                        int* row,
                        uint8_t rclue) -> void
        {
            s << clue_as_char(lclue) << "| ";
            for (uint8_t i = 0; i < N; ++i)
                s << row[i] << ' ';
            s << "|" << clue_as_char(rclue) << '\n';
        };

    auto print_line = [](std::ostream& s) -> void
        {
            s << " +" << std::string(N*2, '-') << "-+ \n";
        };

    std::stringstream ss;
    print_clues(ss, clues_, 0);
    print_line(ss);
    for (uint8_t i = 0; i < N; ++i)
    {
        auto mtx = storage_[matrix_];
        print_row(ss, clues_[N*4-1-i], (*mtx)[i], clues_[N+i]);
    }
    print_line(ss);
    print_clues(ss, clues_, N*2);
    return ss.str();
}

template<class FwIt>
uint8_t
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
std::ostream&
print(std::ostream& s, const std::array<uint8_t, N>& arr)
{
    s << "{";
    std::string delim;
    for (const auto& v : arr)
    {
        s << delim << (int)v;
        delim = ", ";
    }
    s << "};";
    return s;
}

std::ostream& operator<<(std::ostream& s, const std::array<uint8_t, 9>& arr) { print<9>(s, arr); return s; }
std::ostream& operator<<(std::ostream& s, const std::array<uint8_t, 4>& arr) { print<4>(s, arr); return s; }
std::ostream& operator<<(std::ostream& s, const std::array<uint8_t, 6>& arr) { print<6>(s, arr); return s; }
std::ostream& operator<<(std::ostream& s, const std::array<uint8_t, 7>& arr) { print<7>(s, arr); return s; }

template<uint8_t N>
std::ostream&
print(std::ostream& s, const std::vector<std::array<uint8_t, N>>& mtx)
{
    if (mtx.size() < N)
        return s;
    s << "{ ";
    std::string delim;
    for (const auto& arr : mtx)
    {
        s << delim << arr;
        delim = ",\n  ";
    }
    s << " };";
    return s;
}
std::ostream& operator<<(std::ostream& s, const std::vector<std::array<uint8_t, 9>>& mtx) { print<9>(s, mtx); return s; }
std::ostream& operator<<(std::ostream& s, const std::vector<std::array<uint8_t, 4>>& mtx) { print<4>(s, mtx); return s; }
std::ostream& operator<<(std::ostream& s, const std::vector<std::array<uint8_t, 6>>& mtx) { print<6>(s, mtx); return s; }
std::ostream& operator<<(std::ostream& s, const std::vector<std::array<uint8_t, 7>>& mtx) { print<7>(s, mtx); return s; }

template<uint8_t N>
SkyScrappers<N>::SkyScrappers()
{
    if (N > 9)
        return;

    // put zero element into storage as zero solution
    storage_.emplace_back(new std::array<int*, N>{});
    for (uint8_t i = 0; i < N; ++i)
    {
        row_storage_.emplace_back(new std::array<int, N>{});
        storage_.back()->at(i) = row_storage_.back()->data();
    }

    std::array<uint8_t, N> v{};
    for (uint8_t i = 0; i < N; ++i)
        v[i] = i + 1;

    while(std::next_permutation(v.begin(), v.end()))
    {
        uint8_t vis = vision(v.begin(), v.end());
        if (permutations_[vis] == nullptr)
            permutations_[vis].reset(new std::vector<Permutation>{});
        permutations_[vis]->emplace_back(v, vision(v.rbegin(), v.rend()));
        //LOG(INFO) << v << " " << (int)permutations_[vis]->back().rev_vision << std::endl;
    }
}

template<uint8_t N>
inline typename SkyScrappers<N>::Matrix
SkyScrappers<N>::fill(Matrix m, std::array<uint8_t, N> line, int idx)
{
    uint8_t fixed = (idx<2*N ? idx%N : N-1-idx%N);
    uint8_t direction = idx/N;
    if (direction==1 || direction==2)
        std::reverse(line.begin(), line.end());
    bool is_vertical = (direction%2==0);

    for (uint8_t i = 0; i < N; ++i)
    {
        uint8_t& v = (is_vertical ? m[i][fixed] : m[fixed][i]);
        if (v != 0 && v != line[i])
            return {};
        v = line[i];
    }

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
std::vector<typename SkyScrappers<N>::Matrix>
SkyScrappers<N>::generate(std::array<uint8_t, N*4> clues, int i,
                          const std::vector<SkyScrappers<N>::Matrix>& templates)
{
    if (clues[i] == 0)
        return templates;

    if (!permutations_[clues[i]])
        return {};

    std::vector<Matrix> rs;
    auto ri = ridx(i);
    std::vector<Permutation> lines;
    if (clues[ri] == 0)
    {
        lines = *permutations_[clues[i]];
    }
    else
    {
        std::copy_if(permutations_[clues[i]]->begin(), permutations_[clues[i]]->end(),
                     std::back_inserter(lines),
                     [&ri, &clues](const Permutation& permutation)
                     {
                         return permutation.rev_vision == clues[ri];
                     });
    }

    for (const auto& tpl : templates)
    {
        for (const auto line: lines)
        {
            auto matrix = fill(tpl, line.line, i);
            if (!matrix.empty())
                rs.emplace_back(matrix);
        }
    }
    return rs;
}

template<uint8_t N>
typename SkyScrappers<N>::Matrix
SkyScrappers<N>::fill(SkyScrappers<N>::Matrix matrix, uint8_t n, uint8_t i, uint8_t j)
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
std::vector<typename SkyScrappers<N>::Matrix>
SkyScrappers<N>::generate(uint8_t i, uint8_t j, const std::vector<SkyScrappers<N>::Matrix>& templates)
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
int**
SkyScrappers<N>::Solve(const int* clues)
{
    if (N > 9)
        return nullptr;

    Matrix matrix{N};
    for (uint8_t i = 0; i < N*4; ++i)
        clues_[i] = static_cast<uint8_t>(clues[i]);

    uint8_t best_prev = 0;

    // The line for clue = N is determined, we should fill it
    auto clueN = std::find(clues, clues + 4*N, N);
    if (clueN != clues + 4*N)
    {
        Line line;
        for (uint8_t i = 0; i < N; ++i)
            line[i] = i + 1;
        matrix = fill(matrix, line, clueN - clues);
        best_prev = clueN-clues;
    }

    // matrix cell for clue = 1 is determined, we should fill it too
    auto clue1_ = std::find(clues, clues + 4*N, 1);
    if (clue1_ != clues + 4*N)
    {
        auto clue1 = clue1_ - clues;
        if (clue1 < N)
            matrix[0][clue1] = N;
        if (clue1 > N && clue1 < 2*N)
            matrix[clue1%N][N-1] = N;
        if (clue1 > 2*N && clue1 < 3*N)
            matrix[N-1][N-1-clue1%N] = N;
        if (clue1 > 3*N && clue1 < 4*N)
            matrix[N-1-clue1%N][0] = N;
    }


    auto find = [](const std::list<uint8_t>& list, uint8_t val) -> std::list<uint8_t>::const_iterator
    {
        auto i = list.begin();
        for (; i != list.end(); ++i)
            if (val == *i)
                break;
        return i;
    };
    std::list<uint8_t> pending;
    for (uint8_t i = 0; i < clues_.size(); ++i)
        if (clues_[i] != 0 && clues_[i] != N)
            pending.emplace_back(i);


    std::vector<Matrix> candidates{matrix};
    while (!pending.empty())
    {
        if (best_prev != 0)
            pending.sort([best_prev](const uint8_t& lhv, const uint8_t& rhv) { return (lhv/N%2) != (best_prev/N%2); });
        auto best_pending = pending.begin();
        std::vector<Matrix> best_candidates;
        size_t min_sz = std::numeric_limits<size_t>::max();
        std::list<uint8_t> rskip;
        for(auto cur = pending.begin(); cur != pending.end(); ++cur)
        {
            if (find(rskip, *cur) != rskip.end())
                continue;

            auto tmp = generate(clues_, *cur, candidates);
            LOG(INFO) << "   check " << (int)*cur << ": " << tmp.size() << std::endl;
            if (clues_[ridx(*cur)] != 0)
                rskip.emplace_back(ridx(*cur));
            if (tmp.size() < min_sz && tmp.size() > 0)
            {
                best_candidates = tmp;
                min_sz = tmp.size();
                best_pending = cur;
                best_prev = *cur;
                if (min_sz <= candidates.size()*4)
                    break;
            }
        }
        candidates = best_candidates;
        LOG(INFO) << "generated " << (int)*best_pending << "(" << (int)clues_[*best_pending] << "): " << candidates.size() << std::endl;
        pending.erase(best_pending);
        if (clues_[ridx(best_prev)] != 0)
        {
            auto tmp = find(pending, ridx(best_prev));
            if (tmp != pending.end())
                pending.erase(tmp);
        }
    }

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

    return nullptr;
}
