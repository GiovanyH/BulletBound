#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#pragma warning(disable: 4996)

#include <string>
#include <cstdlib>

class gio_filesystem
{
private:
    typedef std::string(*Builder) (const std::string& path);

public:
    static std::string getPath(const std::string& path)
    {
        static std::string(*pathBuilder)(std::string const&) = getPathBuilder();
        return (*pathBuilder)(path);
    }

private:
    static std::string const& getRoot()
    {
        static char const* logl_root = "C:/Users/Owner/OneDrive/Desktop/BulletBound";
        static char const* envRoot = std::getenv("LOGL_ROOT_PATH");
        static char const* givenRoot = (envRoot != nullptr ? envRoot : logl_root);
        static std::string root = (givenRoot != nullptr ? givenRoot : "");
        return root;
    }

    //static std::string(*foo (std::string const &)) getPathBuilder()
    static Builder getPathBuilder()
    {
        if (getRoot() != "")
            return &gio_filesystem::getPathRelativeRoot;
        else
            return &gio_filesystem::getPathRelativeBinary;
    }

    static std::string getPathRelativeRoot(const std::string& path)
    {
        return getRoot() + std::string("/") + path;
    }

    static std::string getPathRelativeBinary(const std::string& path)
    {
        return "../../../" + path;
    }


};

// FILESYSTEM_H
#endif