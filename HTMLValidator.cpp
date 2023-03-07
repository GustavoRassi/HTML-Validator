/********************************************************
*   Project: HTML Validator using a stack and two sets
*   Author: Gustavo A. Rassi
*   Coursework: Data Structures & Algorithms
*********************************************************
* Description: Program designed to read an analyze tags 
*              of an html file to determine if they are 
*              placed and written correctly.
********************************************************/

#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include "LinkedStack.h"
#include "DynamicSet.h"
using namespace std;

int main()
{
    LinkedStack<string> tags; // Stack to store the non self-closing tags
    DynamicSet<string> validTags, selfTags; // Two sets to store the valid tags
    string tag = ""; // Used to store the tags in the Sets and for tag validation later on
    string currentTag = ""; // Used for traversing the lines in the file, character by character

    // Store all valid tags in a Set
    ifstream valid("tags.txt");
    while(getline(valid, tag))
        validTags.add(tag);
    
    // Store only the self-closing tags in another Set
    ifstream selfClosing("self-closing.txt");
    while(getline(selfClosing, tag))
        selfTags.add(tag);

    /* 
     Reset 'tag' if it's going to be used to validate the tags later on.
     Why? Because 'tag' would already have a tag before storing the characters
     for validation and the code will not work. 
    */
    tag = "";

    int lineNumber = 1; // Keeps track of the current line to make reference when there's an error
    bool error = false;
    bool selfclsngError = false;
    
    ifstream import("index.html"); // Link with the HTML file
    if(!import.is_open()) // File doesn't exist, so don't proceed with the program
    {
        cout << "\nFile does not exist or has the wrong path\n" << endl;
        return 0;
    }

    // Get the first line and check if it has the starting tag (<!DOCTYPE html>)
    getline(import, currentTag);
    if(currentTag == "<!DOCTYPE html>") // First line is correct. Now, go through the rest of the file
    {
        while(getline(import, currentTag) && !error && !selfclsngError)
        {
            lineNumber++;

            // Traverse the string, character by character
            for(int i = 0; currentTag[i] != '\0'; i++)
            {
                // Skips ' ' characters until it meets a '<'
                if(currentTag[i] == '<')
                {
                    /* Go one position forward again, since it's a closing tag to prevent false validation */
                    i++;
                    if(currentTag[i] == '/') // It's a closing tag
                    {
                        i++; // Go one position forward again
                        while(currentTag[i] != '>' && currentTag[i] != ' ') 
                        {
                            // Store the characters in a string to get the closing tag
                            tag += currentTag[i];
                            i++;
                        }
                        if(validTags.isElement(tag) && !selfTags.isElement(tag)) // Tag is valid?
                        {
                            // If the tag matches with the most recent in the stack, close it
                            if(tag == tags.top())
                                tags.pop();
                            else
                            {
                                // Not the correct closing tag
                                error = true;
                                break;
                            }
                        }
                        else if(selfTags.isElement(tag)) // Trying to close a self-closing tag
                        {
                            selfclsngError = true;
                            break;
                        }
                        else // Tag is not valid nor is correct
                        {
                            error = true;
                            break;
                        }
                    }
                    else // It's not a closing tag
                    {
                        while(currentTag[i] != '>' && currentTag[i] != ' ')
                        {
                            // Extract the tag to a string variable without spaces or '<>'
                            tag += currentTag[i];
                            i++;
                        }
                        // It's valid but not a self-closing tag, so a tag has opened
                        if(validTags.isElement(tag) && !selfTags.isElement(tag))
                            tags.push(tag);
                        
                        // Tag doesn't exist or written incorrectly, so there's an error
                        if(!validTags.isElement(tag) && !selfTags.isElement(tag))
                        {
                            error = true;
                            break;
                        }
                    }
                }
                // Reset string variable to avoid mixing or combining tags
                tag = "";
            }
    
        }
        if(error) // There's an invalid or incorrect syntax in the file
            cout << "\nError in line " << lineNumber << ": Invalid or missing tag with '" << tag << "'\n";

        else if (selfclsngError) // A self-closing tag is trying to get closed
            cout << "\nError in line " << lineNumber << ": '"<< tag << "' is a self-closing tag\n";

        // Opening tags are left unclosed
        else if(!tags.isEmpty())
            cout << "Error in line " << lineNumber << ": '" << tags.top() << "' must have its closing tag\n";
        
        // All tags are valid and correct with no issues
        else
            cout << "\nCompiled successfully: HTML file is valid!\n" << endl;
    }
    // Tag '<!DOCTYPE html>' is not present in the beginning
    else
        cout << "\nError: DOCTYPE must be in line 1\n" << endl;

    return 0;
}