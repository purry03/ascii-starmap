#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>

std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

class Star
{
public:
    inline Star(std::string id, double ra, double dec,double mag)
        : m_ID(id),
          m_RA(ra),
          m_DEC(dec),
          m_MAG(mag)
    {
    }

    void print()
    {
        std::cout << "ID: HIP-" << m_ID << " RA: " << m_RA << " DEC: " << m_DEC << " MAG: " << m_MAG << std::endl;
    }

private:
    std::string m_ID;
    double m_RA;
    double m_DEC;
    double m_MAG;
};

int main()
{
    const char *star_db_path = "data/hip_main.dat";
    std::vector<Star> stars{};

    // Open the file in text mode
    std::ifstream db_stream(star_db_path);
    if (!db_stream)
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(db_stream, line))
    {
        if(trim(line).length() == 0){
            break;
        }
        try
        {
            // Extract fields using fixed-width offsets
            std::string id = trim(line.substr(2, 12));   // Columns 3-8: ID
            std::string ra_str = line.substr(17, 11);   // Columns 18-28: RA
            std::string dec_str = line.substr(29, 11);  // Columns 30-40: Dec
            std::string mag_str = trim(line.substr(41, 5));   // Columns 42-46: Magnitude

            // Parse RA (HH MM SS.SSS)
            int ra_hh = std::stoi(ra_str.substr(0, 2));
            int ra_mm = std::stoi(ra_str.substr(3, 2));
            float ra_ss = std::stof(ra_str.substr(6, 4));
            double ra_deg = ra_hh * 15 + ra_mm * 0.25 + ra_ss * 0.00416667;

            // Parse Dec (DD MM SS.SSS)
            int dec_sign = (dec_str[0] == '-') ? -1 : 1;
            int dec_hh = std::stoi(dec_str.substr(1, 2));
            int dec_mm = std::stoi(dec_str.substr(4, 2));
            float dec_ss = std::stof(dec_str.substr(7, 4));
            double dec_deg = dec_sign * (dec_hh + dec_mm / 60.0 + dec_ss / 3600.0);

            // Parse Magnitude

            if(mag_str.find_first_not_of(' ') != std::string::npos){
                double mag = std::stod(mag_str);
                stars.emplace_back(id, ra_deg, dec_deg, mag);
            }
            else{
                continue;
            }

            
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception occured: " << e.what() << std::endl;
            break;
        }
    }

    std::cout << "Stars processed: " << stars.size() << std::endl;

    if (!stars.empty())
    {
        stars.at(0).print();
        std::cout << (sizeof(stars[0]) * stars.size())/1024/1024 << "MB" << std::endl;
    }

    return 0;
}