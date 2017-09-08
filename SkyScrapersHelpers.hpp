inline size_t
timestamp()
{
    namespace tm = std::chrono;
    return tm::duration_cast<tm::milliseconds>(tm::steady_clock::now().time_since_epoch()).count();
}

#define INFO "INFO"
#define ERROR "ERROR"
#define LOG(X) std::cout << (X) << " " << timestamp() << ": "

template<uint8_t N>
std::string
print(int** matrix, std::array<int, N*4> clues = {})
{
    if (!matrix)
        return "";

    auto clue_as_char =
        [](uint8_t clue) -> char
        {
            return ((clue > 0 && clue <= 9) ? clue + '0' : ' ');
        };

    auto print_clues =
        [&clue_as_char](std::ostream& s, std::array<int, N*4> clues, int start) -> void
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
    print_clues(ss, clues, 0);
    print_line(ss);
    for (uint8_t i = 0; i < N; ++i)
        print_row(ss, clues[N*4-1-i], matrix[i], clues[N+i]);
    print_line(ss);
    print_clues(ss, clues, N*2);
    return ss.str();
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

