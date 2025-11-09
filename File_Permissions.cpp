#include <iostream>
#include <filesystem>
#include <cstdlib> // For system()

namespace fs = std::filesystem;

/**
 * @brief Sets file permissions using a Unix-style octal mode.
 * @param path The file path.
 * @param mode The octal permission mode (e.g., 755).
 */
void set_file_permissions(const std::string& path, unsigned int mode) {
    try {
#ifdef _WIN32
        // On Windows, use icacls for full security descriptor control
        // Note: This requires understanding the icacls syntax for specific users/groups.
        // A simple example for general read-only access (for all) is difficult with a simple numeric mode
        // For demonstration, we use a basic read-only flag if mode indicates restricted access.
        if (mode == 400 || mode == 600) {
            std::string command = "icacls \"" + path + "\" /inheritance:r /grant:r Users:(R)";
            std::system(command.c_str());
        } else {
            // For full control (777), grant full control to Users
            std::string command = "icacls \"" + path + "\" /grant Users:(F)";
            std::system(command.c_str());
        }
#else
        // On Linux/macOS, use the standard C++17 function with the correct flags
        fs::perms p = fs::perms::none;
        if (mode & 0400) p |= fs::perms::owner_read;
        if (mode & 0200) p |= fs::perms::owner_write;
        if (mode & 0100) p |= fs::perms::owner_exec;
        if (mode & 0040) p |= fs::perms::group_read;
        if (mode & 0020) p |= fs::perms::group_write;
        if (mode & 0010) p |= fs::perms::group_exec;
        if (mode & 0004) p |= fs::perms::others_read;
        if (mode & 0002) p |= fs::perms::others_write;
        if (mode & 0001) p |= fs::perms::others_exec;
        
        fs::permissions(path, p, fs::perm_options::replace);
#endif
        std::cout << "Permissions set for: " << path << std::endl;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error setting permissions for " << path << ": " << e.what() << std::endl;
    }
}
