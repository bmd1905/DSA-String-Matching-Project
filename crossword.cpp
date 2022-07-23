#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

struct coord
{
    int x, y;
};
struct Info
{
    string word;
    coord pos;
    string dr;
};
struct infoOutput
{
    int count;
    vector<Info> foundInfo;
};
void Input(fstream &file, int &h, int &w, int &stringNum, string *matchString)
{
    file.open("input.txt", ios::out);
    cout << "Input width: ";
    cin >> w;
    cout << "Input height: ";
    cin >> h;
    file << w << " " << h << endl;
    char **matrixChar;
    matrixChar = new char *[h];
    for (int i = 0; i < h; i++)
    {
        matrixChar[i] = new char[w];
    }
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
            matrixChar[i][j] = 65 + rand() % 26;
    }

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
            file << matrixChar[i][j] << " ";
        file << endl;
    }
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
            cout << matrixChar[i][j] << " ";
        cout << endl;
    }
    cout << "How many strings do you want to enter? >> ";
    cin >> stringNum;
    matchString = new string[stringNum];
    for (int i = 0; i < stringNum; i++)
    {
        cin >> matchString[i];
    }
    for (int i = 0; i < stringNum; i++)
    {
        for (int j = 0; j < matchString[i].length(); j++)
        {
            matchString[i][j] = toupper(matchString[i][j]);
        }
        file << matchString[i] << endl;
    }
    file << "#";
    file.close();
}
// Algorithms
// Naive
void NaivePatternSearching(string pat, string txt, int &count, int rowNum, int axis, bool &isFound, Info &info)
{
    int M = pat.length(); // Length of word
    int N = txt.length(); // Length of code-word

    int step = N - M;
    for (int i = 0; i <= step; i++)
    {
        int j;

        for (j = 0; j < M; j++)
            if (txt[i + j] != pat[j])
                break;

        if (j == M)
        {
            count++;
            isFound = true;

            if (axis == 0) // row
            {
                info.dr = "LR";
                info.pos = {rowNum, i};

                cout << pat << " ";
                cout << "(" << rowNum << ", " << i << ")"
                     << " LR\n";
            }
            else if (axis == 1) // axis == 1: column
            {
                info.dr = "TD";
                info.pos = {i, rowNum};

                cout << pat << " ";
                cout << "(" << i << ", " << rowNum << ")"
                     << " TD\n";
            }
            else
            {
                info.dr = "NF";
                info.pos = {0, 0};
            }
        }
    }
}
// Rabin Karp
void RabinKarpPatternSearching(string pat, string txt, int &count, int tableSize, int rowNum, int axis, bool &isFound, Info &info)
{
#define d 256
    int M = pat.length();
    int N = txt.length();
    int i, j;
    int p = 0; // hash value for pattern
    int t = 0; // hash value for txt
    int h = 1;

    // The value of h would be "pow(d, M-1)%q"
    for (i = 0; i < M - 1; i++)
        h = (h * d) % tableSize;
    // Calculate the hash value of pattern and first
    // window of text
    for (i = 0; i < M; i++)
    {
        p = (d * p + pat[i]) % tableSize;
        t = (d * t + txt[i]) % tableSize;
    }
    // Slide the pattern over text one by one
    for (i = 0; i <= N - M; i++)
    {
        // Check the hash values of current window of text
        // and pattern. If the hash values match then only
        // check for characters one by one
        if (p == t)
        {
            /* Check for characters one by one */
            for (j = 0; j < M; j++)
            {
                if (txt[i + j] != pat[j])
                {
                    // static int count = 0;
                    // cout << count << " ";
                    // count++;
                    break;
                }
            }
            // if p == t and pat[0...M-1] = txt[i, i+1, ...i+M-1]
            if (j == M)
            {
                count++;
                isFound = true;

                if (axis == 0) // row
                {
                    info.dr = "LR";
                    info.pos = {rowNum, i};

                    cout << pat << " ";
                    cout << "(" << rowNum << ", " << i << ")"
                         << " LR\n";
                }
                else if (axis == 1) // axis == 1: column
                {
                    info.dr = "TD";
                    info.pos = {i, rowNum};

                    cout << pat << " ";
                    cout << "(" << i << ", " << rowNum << ")"
                         << " TD\n";
                }
                else
                {
                    info.dr = "NF";
                    info.pos = {0, 0};
                }
            }
            // cout << "Pattern found at index " << i << endl;
        }
        // Calculate hash value for next window of text: Remove
        // leading digit, add trailing digit
        if (i < N - M)
        {
            t = (d * (t - txt[i] * h) + txt[i + M]) % tableSize;
            // We might get negative value of t, converting it
            // to positive
            if (t < 0)
                t = (t + tableSize);
        }
    }
}
// KMP
void computeLPSArray(string pat, int M, int *lps)
{
    // length of the previous longest prefix suffix
    int len = 0;
    lps[0] = 0; // lps[0] is always 0

    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M)
    {
        if (pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else // (pat[i] != pat[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (len != 0)
            {
                len = lps[len - 1];
                // Also, note that we do not increment
                // i here
            }
            else // if (len == 0)
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}
void KMPPatternSearch(string pat, string txt, int &count, int rowNum, int axis, bool &isFound, Info &info)
{
    int M = pat.length();
    int N = txt.length();

    // create lps[] that will hold the longest prefix suffix
    // values for pattern
    int lps[M];

    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pat, M, lps);

    int i = 0; // index for txt[]
    int j = 0; // index for pat[]
    while ((N - i) >= (M - j))
    {
        if (pat[j] == txt[i])
        {
            j++;
            i++;
        }
        if (j == M)
        {
            count++;
            isFound = true;

            if (axis == 0) // row
            {
                info.dr = "LR";
                info.pos = {rowNum, i - j};

                cout << pat << " ";
                cout << "(" << rowNum << ", " << i - j << ")"
                     << " LR\n";
            }
            else if (axis == 1) // axis == 1: column
            {
                info.dr = "TD";
                info.pos = {i - j, rowNum};

                cout << pat << " ";
                cout << "(" << i - j << ", " << rowNum << ")"
                     << " TD\n";
            }
            else
            {
                info.dr = "NF";
                info.pos = {0, 0};
            }
            j = lps[j - 1];
        }
        // Mismatch after j matches
        else if (i < N && pat[j] != txt[i])
        {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }
}
void WriteToFile(infoOutput info)
{
    fstream fileOut;
    fileOut.open("output.txt", ios::out);
    if (fileOut)
    {
        fileOut << info.count << endl;
        for (int i = 0; i < info.foundInfo.size(); i++)
        {
            fileOut << info.foundInfo[i].word << " ";
            fileOut << "(" << info.foundInfo[i].pos.x << "," << info.foundInfo[i].pos.y << ") ";
            fileOut << info.foundInfo[i].dr << endl;
        }
    }
    else
    {
        cout << "Cann't create output file!\n";
        return;
    }
    fileOut.close();
}
void Output(fstream &file)
{
    file.open("input.txt", ios::in);
    string *row, *rowTranspose;
    string firstLine;
    vector<string> matchString;
    infoOutput info;

    int w, h;
    file >> w;
    file >> h;
    getline(file, firstLine); // ?
    row = new string[h];
    rowTranspose = new string[w];
    for (int i = 0; i < h; i++)
        getline(file, row[i]);

    for (int i = 0; i < h; i++)
        row[i].erase(remove(row[i].begin(), row[i].end(), ' '), row[i].end());

    // Create transpose matrix
    for (int i = 0; i < w; i++)
    {
        string temp = "";
        for (int j = 0; j < h; j++)
            temp.push_back(row[j][i]);
        rowTranspose[i] = temp;
    }

    string buf;
    int matchStringNum = 0;
    while (getline(file, buf))
    {
        if (buf == "#")
            break;
        matchString.push_back(buf);
        matchStringNum++;
    }
    int count = 0;
    int tableSize = INT_MAX;
    for (int i = 0; i < matchStringNum; i++) // Slide each word from input file
    {
        bool isFound = false;
        Info temp;

        // Search with normal matrix
        for (int j = 0; j < h; j++) // Slide each row of normal matrix
        {
            // NaivePatternSearching(matchString[i], row[j], count, j, 0, isFound, temp);
            //  RabinKarpPatternSearching(matchString[i], row[j], count, tableSize, j, 0, isFound, temp);
            KMPPatternSearch(matchString[i], row[j], count, j, 0, isFound, temp);
        }
        // Search with transpose matrix
        for (int j = 0; j < w; j++) // Slide each row of transpose matrix
        {
            // NaivePatternSearching(matchString[i], rowTranspose[j], count, j, 1, isFound, temp);
            //  RabinKarpPatternSearching(matchString[i], rowTranspose[j], count, tableSize, j, 1, isFound, temp);
            KMPPatternSearch(matchString[i], rowTranspose[j], count, j, 1, isFound, temp);
        }
        temp.word = matchString[i];
        if (isFound == false)
        {
            temp.pos = {0, 0};
            temp.dr = "NF";
            cout << matchString[i] << " ";
            cout << "(" << 0 << ", " << 0 << ")"
                 << " NF\n";
        }
        info.foundInfo.push_back(temp);
    }
    cout << count << endl;
    info.count = count;
    file.close();

    WriteToFile(info);
}

int main()
{
    fstream file;
    int height, width, stringNum;
    string *matchString;
    srand((int)time(0));
    char **matrixChar;
    Input(file, height, width, stringNum, matchString);
    Output(file);
    return 0;
}