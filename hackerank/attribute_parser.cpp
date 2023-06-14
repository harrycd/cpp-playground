/*
 * @file attribute_parser.cpp
 * @brief Solution for HackerRank attribute parser problem
 * https://www.hackerrank.com/challenges/attribute-parser/problem
 *
 * @copyright GNU General Public License v3.0
 *
 * @author Theocharis Alexopoulos <printf("tacontact%sgmail%scom","@",".");>
 *
 */

#include <vector>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

/// @brief keeps all tag related information plus utility functions
class Tag
{
public:
    string name;
    int id;
    int parentId;
    vector<int> childrenIds;
    map<string, string> attributes;

    Tag(int idGen, int parentId, string tagName)
    {
        this->id = idGen;
        this->parentId = parentId;
        this->name = tagName;
    }

    /// @brief inserts a new attribute to the attributes map
    /// @param name attribute name
    /// @param value attribute value
    void addAttribute(string name, string value)
    {
        attributes.insert(pair<string, string>(name, value));
    }

    /// @brief retrieves the value for the specified attribute
    /// @param attrName name of the attribute to retrieve the value from
    /// @return the retrieved value. If attribute is not found then Not Found! is returned
    string getAttributeValue(string attrName)
    {
        string result = attributes[attrName];
        if (result == "")
        {
            result = "Not Found!";
        }
        return result;
    }

    /// @brief prints all attributes of the Tag (for debugging purposes)
    void showAttributes()
    {
        for (pair<string, string> attr : attributes)
            cout << "\t" << attr.first << " = " << attr.second << endl;
    }
};

/// @brief
/// @param tagPool
/// @param tagId
/// @return
Tag getTag(vector<Tag> &tagPool, int tagId)
{
    // cout << "searching for tag id " << tagId << "...";
    for (Tag t : tagPool)
    {
        if (t.id == tagId)
        {
            // cout << "found it." << endl;
            return t;
        }
    }
    // cout << "NULL" << endl;
    return Tag(0, 0, "NULL");
}

/// @brief prints the names of the Tags and their corresponding attributes
/// It is used for debugging purposes
///
/// @param tagPool
void printTagPool(vector<Tag> tagPool)
{
    for (Tag t : tagPool)
    {
        cout << t.id << "\t" << t.name;
        if (t.parentId != 0)
        {
            Tag parent = getTag(tagPool, t.parentId);
            cout << "\tchild of: " << parent.name;
        }
        cout << endl;
        t.showAttributes();
    }
}

/// @brief Returns the a tag from the provided tag pool which has
/// the specified parent and name
///
/// @param tagPool the pool to retrieve the tag from
/// @param parentTag the tag to get the parent id from
/// @param tagName the name of the tag to find
/// @return the retrieved Tag. If not found an empty Tag with name NULL is returned.
Tag getTag(vector<Tag> &tagPool, Tag parentTag, string tagName)
{
    // cout << "searching for parent id " << parentTag.id << " and name " << tagName << "...";
    for (Tag t : tagPool)
    {
        if ((t.parentId == parentTag.id) && tagName == t.name)
        {
            // cout << "found it" << endl;
            return t;
        }
    }
    // cout << "NULL" << endl;
    return Tag(0, 0, "NULL");
}

/// @brief finds the vector index in pointing to the Tag with the specified tag id
/// @param tagPool a vector containing tags
/// @param tagId the id of the tag whose index should be returned
/// @return the index of the tag in the pool. If not found the 0 is returned
int getTagPoolIndex(vector<Tag> tagPool, int tagId)
{
    int index = 0;
    for (Tag t : tagPool)
    {
        if (t.id == tagId)
        {
            break;
        }
        index++;
    }
    return index;
}

int main()
{
    vector<Tag> tagPool; // contains all Tags
    int idGen = 0;       // unique id generator for each Tag

    // Reading the first line with getline ensures all characters are
    // read and the second call to getline will resume from line 2
    string lineStr;
    getline(cin, lineStr);
    stringstream sss(lineStr);
    int lines, queries;
    sss >> lines >> queries;

    // Create a fictional root Tag to attach to it the Tag hierarchy
    Tag rootTag = Tag(++idGen, 0, "root_000");
    tagPool.push_back(rootTag);
    Tag &currentTag = rootTag;

    for (int i = 0; i < lines; i++)
    {
        // Read the HRML code line by line
        getline(cin, lineStr);
        stringstream ss(lineStr);
        string s;

        while (ss >> s)
        {
            if (s.substr(0, 1) == "<" && s.substr(0, 2) != "</")
            {
                // This is a new tag

                // Remove '<' and '>' character to keep Tag name
                string tagName = s.substr(1, s.size() - 1);
                if (tagName.substr(tagName.size() - 1, 1) == ">")
                {
                    tagName = tagName.substr(0, tagName.size() - 1);
                }

                // Create the new tag and add it to its parent (currentTag) and the pool
                Tag tag = Tag(++idGen, currentTag.id, tagName);
                (currentTag).childrenIds.push_back(tag.id);
                tagPool.push_back(tag);
                currentTag = tag;
            }
            else if (s.substr(0, 1) != "<")
            {
                // This is an attribute name

                string equals, value;
                ss >> equals >> value;
                if (equals != "=")
                {
                    cout << "Error reading equals: " << endl;
                }
                if (value.substr(value.size() - 1, 1) == ">")
                {
                    // The last char of an opening tag
                    value = value.substr(0, value.size() - 1);
                }
                // Remove the quotes "" around attribute name and save
                value = value.substr(1, value.size() - 2);
                tagPool.at(getTagPoolIndex(tagPool, currentTag.id)).addAttribute(s, value);
            }
            else if (s.substr(0, 2) == "</")
            {
                // This is a closing tag
                currentTag = getTag(tagPool, currentTag.parentId);
            }
        }
    }
    for (int i = 0; i < queries; i++)
    {
        // Respond to queries
        string query;
        getline(cin, query);
        Tag targetTag = rootTag;
        while (query.find(".") != string::npos)
        {
            // Process the tag name
            int pos = query.find(".");
            targetTag = getTag(tagPool, targetTag, query.substr(0, pos));
            query = query.substr(pos + 1, query.size() - (pos + 1));
        }
        if (query.find("~") >= 0)
        {
            // This is the last tag name followed by an attribute name
            int pos = query.find("~");
            targetTag = getTag(tagPool, targetTag, query.substr(0, pos));
            query = query.substr(pos + 1, query.size() - (pos + 1));
            cout << targetTag.getAttributeValue(query.substr(0, query.size())) << endl;
        }
    }

    return 0;
}
