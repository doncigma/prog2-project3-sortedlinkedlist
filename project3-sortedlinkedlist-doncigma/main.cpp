// This is the standard main file for the repl and command line parsing.
// DO NOT CHANGE THIS FILE without talking with me first.
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "include/cxxopts.hpp"

#include "helpers.hpp"
#include "unsortedlinkedlist.hpp"
#include "linkedlisttest.hpp"

using namespace std;

bool quit(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine);
bool help(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine);
bool test(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine);
bool debug(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine);

/// @brief Processes a line of input.
/// @param line The line to process.
/// @param commandString The allOutput parameter to store the command itself.
/// @param allOutput The allOutput parameter to store the allOutput of the command.
/// @param expectedOutput The allOutput parameter to store the expected allOutput of the command.
/// @param comment The allOutput parameter to store the comment of the command.
/// @param interactive Whether the command is being run in an interactive session.
/// @param fileName The name of the file being processed.
/// @param currentLine The current line number in the input stream.
/// @return Whether the command was successful.
bool ProcessLineCommand(const string &line, std::string &commandString, std::string &allOutput, std::string &expectedOutput, std::string &comment, bool interactive, string fileName, int currentLine);

/// @brief Processes a command.
/// @param command The command to process.
/// @param params The parameters to pass to the command.
/// @param outputStream The allOutput stream to write the command's allOutput to.
/// @param interactive Whether the command is being run in an interactive session.
/// @param fileName The name of the file being processed.
/// @param currentLine The current line number in the input stream.
/// @return Whether the command was successful.
bool ProcessCommand(const string &command, const vector<string> &params, string &allOutput, bool interactive, string fileName, int currentLine);

// Create an array of pairs, each containing a command name and its associated function
vector<TestFunctionEntry> baseCommands = {
    {"quit", "quit", quit},
    {"help", "help [command] - gives help for the optional command.", help},
    {"test", "test <input file> ... - tests a list of files.", test},
    {"debug", "debug <0|1> - turns debug mode on or off.", debug},
    {"?", "? [command] - gives help for the optional command.", help},
};

std::map<std::string, TestFunctionEntry> commandMap;

void ProcessStream(istream *sourceStream, ostream *outputStream, bool interactive, string fileName)
{
    if (sourceStream == NULL)
    {
        sourceStream = &cin;
        fileName = "cin";
    }

    if (outputStream == NULL)
    {
        outputStream = &cout;
    }

    if (interactive)
    {
        *outputStream << "Simple C++ REPL - Enter an arithmetic expression or 'quit' to quit." << endl;
    }

    int currentLine = 0;

    while (true)
    {
        if (interactive)
        {
            *outputStream << "> ";
        }

        string input;
        if (!getline(*sourceStream, input))
        {
            break;
        }

        currentLine++;

        string command;
        string allOutput;
        string expectedOutput;
        string comment;

        input = trim(input);

        bool b = ProcessLineCommand(input, command, allOutput, expectedOutput, comment, interactive, fileName, currentLine);
        *outputStream << allOutput << endl;

        if (!b)
        {
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    istream *sourceStream = NULL;
    ostream *outputStream = NULL;
    bool interactive = true;
    string fileName = "";

    cxxopts::Options options("REPL", "A simple C++ REPL");

    options.add_options()("e,expression", "Evaluate the provided arithmetic expression", cxxopts::value<string>())("h,help", "Display this help message")("s,source", "Source file to evaluate line by line.", cxxopts::value<string>())("i,interactive", "Source file to evaluate line by line.")("o,allOutput", "Output file to write results to.", cxxopts::value<string>())("d,debug", "Sets debug mode on(1) or off(0).", cxxopts::value<int>());

    AddCommands(baseCommands, commandMap);
    AddCommands(linkedListTestCommands, commandMap);

    try
    {
        cxxopts::ParseResult result = options.parse(argc, argv);

        if (result.count("help"))
        {
            cout << options.help() << endl;
            return EXIT_SUCCESS;
        }

        if (result.count("debug"))
        {
            Debug = result["debug"].as<int>();
        }

        // See if we have an allOutput file to write to.
        if (result.count("allOutput"))
        {
            string ofilename = result["allOutput"].as<string>();
            outputStream = new ofstream(ofilename);

            if (outputStream == NULL || outputStream->fail())
            {
                cerr << "Error creating allOutput stream \"" << ofilename << "\"" << endl;
                return EXIT_FAILURE;
            }
        }

        // If interactive is set then ignore the other source options.
        if (result.count("interactive"))
        {
            interactive = true;
            sourceStream = NULL;
        }
        // Next prioritize setting a command line expression.
        else if (result.count("expression"))
        {
            interactive = false;
            string expression = result["expression"].as<string>();

            sourceStream = new stringstream(expression);

            if (sourceStream == NULL || sourceStream->fail())
            {
                cerr << "Error creating source stream from expression." << endl;
                return EXIT_FAILURE;
            }
        }
        // Next prioritize setting a source file.
        else if (result.count("source"))
        {
            interactive = false;
            fileName = result["source"].as<string>();
            sourceStream = new ifstream(fileName);

            if (sourceStream == NULL || sourceStream->fail())
            {
                cerr << "Error creating source stream \"" << fileName << "\"" << endl;
                return EXIT_FAILURE;
            }
        }
        // Otherwise none were set so make it interactive.
        else
        {
            interactive = true;
            sourceStream = NULL;
        }

        ProcessStream(sourceStream, outputStream, interactive, fileName);
    }
    catch (const cxxopts::exceptions::exception &e)
    {
        cerr << "Error parsing options: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    // Cleanup

    if (sourceStream != NULL)
    {
        delete sourceStream;
        sourceStream = NULL;
    }

    if (outputStream != NULL)
    {
        delete outputStream;
        outputStream = NULL;
    }

    return 0;
}

bool help(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine)
{
    allOutput = "";

    if (params.size() == 0)
    {
        cout << "Available commands: ";

        for (const auto &pair : commandMap)
        {
            cout << pair.first << " ";
        }

        cout << endl;

        return true;
    }
    else if (params.size() == 1)
    {
        auto match = commandMap.find(params[0]);

        if (match == commandMap.end())
        {
            PrintError(currentLine, 0, "Unknown command '" + params[0] + "'");
            return true;
        }

        cout << match->second._name << ": " << match->second._help << endl;
    }
    else
    {
        throw invalid_argument("help requires 0 or 1 parameters");
    }

    return true;
}

bool quit(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine)
{
    if (interactive)
    {
        cout << "Goodbye!" << endl;
    }

    allOutput = "";

    return false;
}

bool testOneFile(string fileName, string &allOutput, bool interactive, int currentLine)
{
    ifstream testInputFile;

    testInputFile.open(fileName);

    if (testInputFile.fail())
    {
        PrintError(currentLine, 0, "Could not open input file '" + fileName + "'");
        allOutput += "\n" + fileName + ": Could not open input file\n";
        return true;
    }

    int fileCurrentLine = 0;
    int errorCount = 0;

    while (true)
    {
        string input;
        if (!getline(testInputFile, input))
        {
            break;
        }

        fileCurrentLine++;

        string command;
        string output;
        string expectedOutput;
        string comment;

        input = trim(input);

        bool b = ProcessLineCommand(input, command, output, expectedOutput, comment, interactive, fileName, fileCurrentLine);

        // Compare allOutput and expectedOutput ignoring leading and trailing spaces
        string outputTrim = trim(output);
        string expectedOutputTrim = trim(expectedOutput);

        if (Debug)
        {
            string sameStr = (outputTrim == expectedOutputTrim) ? "Same" : "Differs";

            PrintDebug(fileName, fileCurrentLine, 0,
                       sameStr + ": Command: \'" + command + "\'; Result: \'" + outputTrim + "\'; Expected: \'" + expectedOutputTrim + "\'");
        }

        if (outputTrim != expectedOutputTrim)
        {
            stringstream sstr;
            sstr << fileName << ":Line " << fileCurrentLine << " differs: Command: \'" << command << "\'; Result: \'" << outputTrim << "\'; Expected: \'" << expectedOutputTrim << "\'" << endl;
            allOutput += sstr.str();

            errorCount++;
        }

        if (!b)
        {
            break;
        }
    }

    allOutput += "\n" + fileName + ":";
    if (errorCount == 0)
    {
        allOutput += "All tests passed\n\n";
    }
    else
    {
        allOutput += "Failed test count - " + to_string(errorCount) + "\n\n";
    }

    return true;
}

bool test(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine)
{
    if (params.size() < 1)
    {
        throw std::invalid_argument("test requires at least one parameter");
    }

    allOutput = "";
    bool bRet = true;
    for (const string &fileName : params)
    {
        string output;
        bRet = testOneFile(fileName, output, interactive, currentLine) && bRet;
        allOutput += output;
    }

    return bRet;
}

bool debug(const std::vector<std::string> &params, string &allOutput, bool interactive, int currentLine)
{
    if (params.size() != 1)
    {
        throw std::invalid_argument("verbose requires one parameter");
    }

    allOutput = "";

    Debug = stoi(params[0]);

    return true;
}

bool ProcessLineCommand(const string &line, std::string &commandString, std::string &allOutput, std::string &expectedOutput, std::string &comment, bool interactive, string fileName, int currentLine)
{
    ParseLine(line, commandString, expectedOutput, comment, ';', '#');

    vector<string> splitLine = SplitString(commandString);
    if (splitLine.empty())
    {
        allOutput = "";
        return true;
    }

    // Remove the command
    string command = trim(splitLine.front());
    splitLine.erase(splitLine.begin());

    return ProcessCommand(command, splitLine, allOutput, interactive, fileName, currentLine);
}

// Return true to continue false to exit
bool ProcessCommand(const string &command, const vector<string> &params, string &allOutput, bool interactive, string fileName, int currentLine)
{
    vector<string> matches = FindPrefixMatch(commandMap, command);

    if (matches.empty())
    {
        ostringstream stringStream;
        stringStream << "Invalid command '" << command << "'"
                     << ". Type '?' or 'help' for a list of commands.";
        PrintError(currentLine, 0, stringStream.str());

        allOutput = "error";
        return true;
    }
    else if (matches.size() == 1 || matches.front() == command)
    {
        try
        {
            return commandMap[matches.front()]._function(params, allOutput, interactive, currentLine);
        }
        catch (const std::invalid_argument &e)
        {
            PrintError(currentLine, 0, "Invalid argument: " + std::string(e.what()));
            allOutput = "error";

            return true;
        }
        catch (const std::out_of_range &e)
        {
            PrintError(currentLine, 0, "Out of range: " + std::string(e.what()));
            allOutput = "error";
            return true;
        }
        catch (const LinkedListException &e)
        {
            PrintDebug(fileName, currentLine, 0, "LinkedList Error: " + std::string(e.what()));

            allOutput = "error";
            return true;
        }
        catch (...)
        {
            PrintError(currentLine, 0, "Unknown exception occurred");
            allOutput = "error";
            return true;
        }
    }
    else
    {
        ostringstream stringStream;
        stringStream << "Ambiguous command '" << command << "'"
                     << ". Did you mean one of these: ";
        for (const auto &match : matches)
        {
            stringStream << match << " ";
        }
        PrintError(currentLine, 0, stringStream.str());
        allOutput = "error";
        return true;
    }
}
