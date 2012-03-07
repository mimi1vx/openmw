#include "filelibrary.hpp"

#include <iostream>

#include <boost/algorithm/string.hpp>

namespace Files
{
    // Looks for a string in a vector of strings
    bool containsVectorString(const StringVector& list, const std::string& str)
    {
        for (StringVector::const_iterator iter = list.begin();
             iter != list.end(); iter++)
        {
            if (*iter == str)
                return true;
        }
        return false;
    }

    // Searches a path and adds the results to the library
    void FileLibrary::add(const boost::filesystem::path &root, bool recursive, bool strict,
            const StringVector &acceptableExtensions)
    {
        PathContainer list;
        std::string fileExtension;
        std::string type;
        FileLister(root, list, recursive);

        for (PathContainer::iterator listIter = list.begin();
            listIter != list.end(); ++listIter)
        {
            if( !acceptableExtensions.empty() )
            {
                fileExtension = listIter->extension().string();
                boost::algorithm::to_lower(fileExtension);
                if(!containsVectorString(acceptableExtensions, fileExtension))
                    continue;
            }

            type = listIter->parent_path().leaf().string();
            if (!strict)
                boost::algorithm::to_lower(type);

            mMap[type].push_back(*listIter);
            //std::cout << "Added path: " << listIter->string() << " in section "<< type <<std::endl;
        }
    }

    // Returns true if the named section exists
    bool FileLibrary::containsSection(std::string sectionName, bool strict)
    {
        if (!strict)
            boost::algorithm::to_lower(sectionName);
        StringPathContMap::const_iterator mapIter = mMap.find(sectionName);
        if (mapIter == mMap.end())
            return false;
        else
            return true;
    }

    // Returns a pointer to const for a section of the library
    const PathContainer* FileLibrary::section(std::string sectionName, bool strict)
    {
        if (!strict)
            boost::algorithm::to_lower(sectionName);
        StringPathContMap::const_iterator mapIter = mMap.find(sectionName);
        if (mapIter == mMap.end())
        {
            //std::cout << "Empty\n";
            return &mEmptyPath;
        }
        else
        {
            return &(mapIter->second);
        }
    }

    // Searches the library for an item and returns a boost path to it
    boost::filesystem::path FileLibrary::locate(std::string item, bool strict, std::string sectionName)
    {
        boost::filesystem::path result("");
        if (sectionName == "")
        {
            for(StringPathContMap::iterator iter = mMap.begin(); iter != mMap.end(); iter++)
            {
                result = FileListLocator(iter->second, boost::filesystem::path(item), strict);
                if (result != boost::filesystem::path(""))
                    return result;
            }
        }
        else
        {
            if (!containsSection(sectionName, strict))
            {
                std::cout << "Warning: There is no section named " << sectionName << "\n";
                return result;
            }
            result = FileListLocator(mMap[sectionName], boost::filesystem::path(item), strict);
        }
        return result;
    }

    // Prints all the available sections, used for debugging
    void FileLibrary::printSections()
    {
        for(StringPathContMap::const_iterator mapIter = mMap.begin();
             mapIter != mMap.end(); mapIter++)
        {
            std::cout << mapIter->first <<std::endl;
        }
    }
}
