#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "too few args" << std::endl;
        return 0;
    }
    const auto pe_file_path = std::string(argv[1]);
    const auto shellcode_save_path = std::string(argv[2]);
    auto file_name = pe_file_path.substr(pe_file_path.find_last_of("\\/") + 1);
    file_name = file_name.substr(0, file_name.find_last_of("."));

    std::ofstream outfile(shellcode_save_path);
    std::ifstream file(pe_file_path, std::ios::binary);
    if (!outfile || !file)
    {
        std::cout << "path args error" << std::endl;
        return 0;
    }
    std::vector<unsigned char> pe_file_data(std::istreambuf_iterator<char>(file), {});
    outfile << "#pragma once" << std::endl;
    outfile << "unsigned char " << file_name << "_data" << "[" << std::to_string(pe_file_data.size()) << "] = {" << std::endl;
    const auto line_feed = 16;
    auto index = 0;
    for (const auto& byte : pe_file_data) {
        if (index == 0)
        {
            outfile << "          ";
        }
        outfile << "0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(byte) << ",";
        index++;
        if (line_feed == index)
        {
            outfile << std::endl;
            index = 0;
        }
    }
    outfile << "};" << std::endl;
    outfile.close();
    return 1;
}
