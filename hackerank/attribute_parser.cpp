#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <map>
#include <fstream>

using namespace std;


class Tag{
    public:
        string name;
        int id;
        int parentId;
        vector<int> childrenIds;
        map<string, string> attributes;
        
        Tag(int idGen, int parentId, string tagName){
            this->id = idGen;
            this->parentId = parentId;
            this->name = tagName;
        }
        
        void addAttribute(string name, string value){
            attributes.insert(pair<string,string>(name,value));
            //cout << "added to " << this->name << " attribute " << name << "=" << value << endl;
        }
        
        string getAttributeValue(string attrName){
            string result = attributes[attrName];
            if (result == ""){
                result = "Not Found!";
            }
            return result;
        }
        
        void showAttributes(){
            //cout << "Attributes of " << this->name << endl;
            for (pair<string, string> attr : attributes)
                cout << "\t" << attr.first << " = " << attr.second << endl;
        }
        
};

Tag getTag(vector<Tag>& tagPool, int tagId);

void printTagPool(vector<Tag> tagPool){
    for (Tag t : tagPool){
        cout << t.id << "\t" << t.name;
        if (t.parentId != 0){
            Tag parent = getTag(tagPool, t.parentId);
            cout << "\tchild of: " << parent.name;
        }
        cout << endl;
        t.showAttributes();
    }
}

Tag getTag(vector<Tag>& tagPool, int tagId){
    //cout << "searching for tag id " << tagId << "...";
    for (Tag t : tagPool){
        if (t.id == tagId){
            //cout << "found it." << endl;
            return t;
        }
    }
    //cout << "NULL" << endl;
    return Tag(0, 0, "NULL");
}

Tag getTag(vector<Tag>& tagPool, Tag parentTag, string tagName){
    //cout << "searching for parent id " << parentTag.id << " and name " << tagName << "...";
    for (Tag t : tagPool){
        if ((t.parentId == parentTag.id) && tagName == t.name){
            //cout << "found it" << endl;
            return t;
        } 
    }
    //cout << "NULL" << endl;
    return Tag(0,0,"NULL");
}

int getTagPoolIndex(vector<Tag> tagPool, int tagId){
    int index = 0;
    for(Tag t : tagPool){
        if (t.id == tagId){
            break;
        }
        index++;
    }
    return index;
}


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */ 
    fstream cin("hrml.txt");

    vector<Tag> tagPool;
    int idGen = 0;
    
    string lineStr;
    getline(cin, lineStr); //To skip the blank line
    stringstream sss(lineStr);
    int lines, queries;
    sss >> lines >> queries;

    Tag rootTag = Tag(++idGen, 0, "root_000");
    tagPool.push_back(rootTag);
    Tag& currentTag = rootTag;
    
    for (int i = 0; i < lines; i++){
        // Read the HRML code line by line
        getline(cin, lineStr);
        stringstream ss(lineStr);
        string s;
        
        
        while(ss >> s){
            //cout << "processing: " << s << endl;
            if (s.substr(0,1) == "<" && s.substr(0,2) != "</"){
                //This is a new tag
                if (s.substr(s.size()-2, 1) == ">"){
                    //no attributes, remove last character
                    s = s.substr(0,s.size()-2);
                }
                //remove '<' character
                string tagName = s.substr(1,s.size()-1);
                if (tagName.substr(tagName.size()-1, 1) == ">"){
                    tagName = tagName.substr(0, tagName.size()-1);
                }
                
                
                Tag tag = Tag(++idGen, currentTag.id, tagName);
                (currentTag).childrenIds.push_back(tag.id); //add to parent
                tagPool.push_back(tag); //add to tagPool
                currentTag = tag;
            }
            else if (s.substr(0,1) != "<"){
                //attribute name
                string equals, value;
                ss >> equals >> value;
                if (equals != "="){
                    cout << "Error reading equals: " << endl;
                }
                if (value.substr(value.size()-1, 1) == ">"){
                    //end of opening tag
                    value = value.substr(0, value.size()-1);
                }
                //remove the quotes "" and save
                value = value.substr(1, value.size()-2);
                tagPool.at(getTagPoolIndex(tagPool, currentTag.id)).addAttribute(s, value);
            }
            else if (s.substr(0,2) == "</"){
                //closing tag
                currentTag = getTag(tagPool, currentTag.parentId);
            }
        }
        
    }
    printTagPool(tagPool);
    cout << "=======================\n\n" << endl; 
    for (int i = 0; i < queries; i++){
        // Respond to queries
        string query;
        cin >> query;
        Tag targetTag = rootTag;
        while(query.find(".") != string::npos){
            int pos = query.find(".");
            //cout << "q.getTag: " << query.substr(0,pos) << endl;
            targetTag = getTag(tagPool, targetTag, query.substr(0,pos));
            query = query.substr(pos+1, query.size()-(pos+1));
        }
        if(query.find("~") >= 0){
            int pos = query.find("~");
            //cout << "q.getTag: " << query.substr(0,pos) << endl;
            targetTag = getTag(tagPool, targetTag, query.substr(0,pos));
            //cout << "returned " << targetTag.name << endl;
            query = query.substr(pos+1, query.size()-(pos+1));
            //cout << "q.getAttr :" << query.substr(0, query.size()) << endl;
            cout << targetTag.getAttributeValue(query.substr(0, query.size())) << endl;
        
    }
    }
    
    
    return 0;
}
