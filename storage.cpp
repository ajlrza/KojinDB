// Headers import first to raise error early and avoid overhead if later
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <map>
#include <filesystem>
#include <string>
#include <streambuf>
#include <fstream>
using namespace std;

struct FileCreationConfig {
    std::filesystem::path file_name;
    std::filesystem::path directory_path;
};

std::string file_operation_fallback(const char* failed_code, const char* file_name, int line_number) {

    const char* failed_code_ptr = failed_code; 
    std::string failed_code_txt;

    while (*failed_code_ptr != '\0') {
        failed_code_txt.push_back(*failed_code_ptr);
        failed_code_ptr++; 
    }
    
    if (failed_code_txt == "create_directory") {
        std::cout << "Cannot create directory.";
    }
    else if (failed_code_txt == "exists") {
        std::cout << "File already exists.";
    };

}

#define assert(condition) \
    if (!(condition)) { \
        return file_operation_fallback(#condition, __FILE__, __LINE__); \
    } \

/**
 * @brief Creates the directory in disk
 * @details Uses the std::filesystem library to manipulate the creation
 * @param directory_name Desired name in std::string data type.
 * @return The directory file path.
 */
std::string CREATE_DIRECTORY(std::string directory_name) {
    
    assert(!std::filesystem::create_directory(directory_name));

    bool check_if = std::filesystem::exists(directory_name);

    if (check_if) { 
       return directory_name;
    }

    try {
        std::filesystem::create_directory(directory_name);
    } catch (std::runtime_error e) {
        std::cout << "Error creating directorry.";
        return e.what();
    }

    std::filesystem::create_directory(directory_name);

    return "/" + directory_name;

}

/**
 * @brief Creates file in disk
 * @details Uses the std::filesystem library to create empty file in disk
 * @param config File config struct instantiated in engine and passed here.
 * @return The file path created.
 */
std::filesystem::path CREATE_FILE(FileCreationConfig config) {

    std::ofstream file_creator;

    bool check_if = std::filesystem::exists(config.file_name);
    bool check_again = std::filesystem::exists(config.directory_path / config.file_name);
    assert(!std::filesystem::exists(config.directory_path / config.file_name));

    if (check_if || check_again || check_if && check_again) { return "File already exists."; };

    if (config.directory_path.empty()) {
        std::filesystem::path file = config.file_name;

        std::filesystem::path file_path = file;
        file_path.replace_extension(".txt");

        file_creator.open(file_path);

        return file_path;        
    }

    std::filesystem::path dr = config.directory_path;
    std::filesystem::path file = config.file_name;

    std::filesystem::path file_path = dr / file;
    file_path.replace_extension(".txt");

    file_creator.open(file_path);

    return file_path;

}

/**
 * @brief Selects directory and traverses them
 * @details Function called when accessing directory is needed
 * @param directory_name Directory name to be accessed or traversed.
 * @return The directory items such as its child items as files.
 */
std::string SELECT_DIRECTORY(std::string directory_name) {
    
    std::fstream directory;

    try {
        directory.open(directory_name);
    } catch (std::runtime_error &e) {
        CREATE_DIRECTORY(directory_name);
        std::cout << "Directory did not exist, created new one.";
        return directory_name;
    }
 
    directory.open(directory_name);

    if (directory.is_open()) {

        std::string directory_items;

        while (std::getline(directory, directory_items)) {
            std::cout << directory_items << std::endl;
        }

        directory.close();

        return directory_items;
    }
}

/**
 * @brief Writes static data to a file
 * @details Uses the "static" data or already defined data to write.
 * @param file_path File path to write the data on.
 * @param read_obj The input string stream containing the data.
 * @return The file path of the file with written data.
 */
std::filesystem::path STATIC_WRITE_TO_FILE(std::filesystem::path file_path, std::istringstream& read_obj) {

    std::fstream file;

    FileCreationConfig config;
    config.file_name = file_path;

    assert(!std::filesystem::exists(file_path));

    try {
        file.open(file_path);
    } catch (std::runtime_error &e) {
        CREATE_FILE(config);
    }

    file.open(file_path);

    if (file.is_open()) {

        std::string file_store;

        while (std::fstream::getline(read_obj, file_store)) {
            file << file_store;
        }

        file.close();

        return file_path;
    }
}

/**
 * @brief Wrtites dynamic data to file
 * @details Uses the "dynamic" data or the input stream from the user.
 * @param file_path File path to write the data on.
 * @return The file path of the file with written data.
 */
std::filesystem::path DYNAMIC_WRITE_TO_FILE(std::filesystem::path file_path) {

    std::fstream file;

    FileCreationConfig config;
    config.file_name = file_path;

    assert(!std::filesystem::exists(file_path));

    try {
        file.open(file_path);
    } catch (std::runtime_error &e) {
        CREATE_FILE(config);
    }

    file.open(file_path);

    if (file.is_open()) {
        
        std::string file_inputs;
        std::cin >> file_inputs;

        for (int character = 0; character < file_inputs.length(); character++) {
            for (char character : file_inputs) {
                file << character;
            }
        }
    }

    return file_path;

}
