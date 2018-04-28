#include <cstdio>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <map>
#include <vector>
#include <streambuf>

#include "utils.hpp"

// constant string table is useful for checking information using "strings" utility.
const char* APPLICATION_NAME = "classgen";
const char* VERSION_MAJOR = "0";
const char* VERSION_MINOR = "1";
const char* VERSION_PATCH = "0";
const char* VERSION = "0.1.0";
const char* COPYRIGHT = "(C) Copyright 2018";
const char* AUTHOR = "Richard Marks";
const char* CONTACT = "ccpsceo@gmail.com";
const char* LICENSE = "MIT";

/*
  classgen application

  given "path/to/classname" as an input on the command line,
  this software will write two text files and generate
  any necessary directories as follows:

  path/to directories will be created if they do not exist
  file Classname.hpp will be written to the path/to/ directory

  #ifndef CLASSNAME_H
  #define CLASSNAME_H

  class Classname {
    // TODO: write the class definition for Classname
  };

  #endif


  file Classname.cpp will be written to the path/to/ directory

  #include "Classname.hpp"

  // TODO: write the class implementation for Classname

*/
class App {
  enum State {
    Help,
    Version,
    Generator
  };

  public:
    App(int argc, char* argv[]);

  private:
    void processArgs(int argc, char* argv[]);
    void runHelpState();
    void runVersionState();
    void runGeneratorState();
    void printBanner();

    std::string execName;
    std::string generatorInput;
    App::State state;
};

App::App(int argc, char* argv[])
  : state(App::State::Help) {
  processArgs(argc, argv);
  switch (state) {
    case App::State::Help: runHelpState(); break;
    case App::State::Version: runVersionState(); break;
    case App::State::Generator: runGeneratorState(); break;
    default: break;
  }
}

void App::processArgs(int argc, char* argv[]) {
  execName = std::string(argv[0]);

  state = State::Help;

  if (argc == 2) {
    std::string argv1 = argv[1];
    if (argv1 == "-v" || argv1 == "--version") {
      state = State::Version;
    } else if (argv1 == "-h" || argv1 == "--help") {
      state = State::Help;
    } else {
      generatorInput = std::string(argv1);
      state = State::Generator;
    }
  }
}

void App::runHelpState() {
  printBanner();
  std::cout
    << "Usage: "
    << execName
    << " ([-h | -v | --help | --version] | {classname})"
    << std::endl
    << std::endl
    << "    -h or --help       display this help screen"
    << std::endl
    << std::endl
    << "    -v or --version    display the software version"
    << std::endl
    << std::endl
    << "    OR"
    << std::endl
    << std::endl
    << "    {classname}        path to the name of the class to generate."
    << std::endl
    << std::endl
    << "Examples:"
    << std::endl
    << std::endl
    << "Use Case 1 - Generate class files in the current directory"
    << std::endl
    << "    " << execName << " example"
    << std::endl
    << std::endl
    << "This command will create the files \"Example.hpp\" and \"Example.cpp\" in the created directory."
    << std::endl
    << std::endl
    << "Use Case 2 - Generate class files in a nested directory"
    << std::endl
    << "    " << execName << " relative/path/to/example"
    << std::endl
    << std::endl
    << "This command will create in the current directory, the path \"relative/path/to/\" if it does not exist."
    << "Next, the files \"Example.hpp\" and \"Example.cpp\" will be created in the created directory."
    << std::endl
    << std::endl;
}

void App::runVersionState() {
  std::cout
    << "v" << VERSION_MAJOR
    << "." << VERSION_MINOR
    << "." << VERSION_PATCH
    << std::endl;
}

void App::runGeneratorState() {
  printBanner();

  std::string basename = utils::basename(generatorInput);
  std::string path = utils::path(generatorInput);
  std::string targetClassname = utils::capitalize(basename);
  std::string targetClassnameCaps = utils::upcase(basename);
  std::string targetClassnameHeaderGuard = targetClassnameCaps + "_H";
  std::string targetClassnameHeader = targetClassname + ".hpp";
  std::string targetClassnameSource = targetClassname + ".cpp";

  #ifdef DEBUG
  std::cout
    << "input: " << generatorInput << std::endl
    << "path: " << path << std::endl
    << "basename: " << basename << std::endl
    << "class name: " << targetClassname << std::endl
    << "class name capitalized: " << targetClassnameCaps << std::endl
    << "class header guard: " << targetClassnameHeaderGuard << std::endl
    << "class header file: " << targetClassnameHeader << std::endl
    << "class source file: " << targetClassnameSource << std::endl;
  #endif

  std::stringstream headerCode;
  std::stringstream sourceCode;

  std::cout
    << "classgen v0.1.0 :: MIT license (C) Copyright 2018, Richard Marks <ccpsceo@gmail.com>" << std::endl
    << "Preparing Directories..." << std::endl;

  if (path != basename) {
    utils::mkdirp(path);
  } else {
    path = ".";
  }

  std::cout << "Generating class " << targetClassname << std::endl;

  std::string headerPath = path + "/" + targetClassnameHeader;
  std::cout << "Writing Header into " << headerPath << std::endl;

  std::ofstream headerFile(headerPath);
  if (!headerFile.is_open()) {
    throw std::runtime_error("Unable to open " + headerPath + " for writing!");
  }

  headerCode
    << "#ifndef " << targetClassnameHeaderGuard << std::endl
    << "#define " << targetClassnameHeaderGuard << std::endl
    << std::endl
    << "class " << targetClassname << " {" << std::endl
    << "  // TODO: write the class definition for " << targetClassname << std::endl
    << "};" << std::endl
    << std::endl
    << "#endif // !" << targetClassnameHeaderGuard << std::endl
    << std::endl;

  std::cout << headerCode.str();
  headerFile << headerCode.str();
  headerFile.close();

  std::string sourcePath = path + "/" + targetClassnameSource;
  std::cout << "Writing Source into " << sourcePath << std::endl;

  std::ofstream sourceFile(sourcePath);
  if (!sourceFile.is_open()) {
    throw std::runtime_error("Unable to open " + sourcePath + " for writing!");
  }

  sourceCode
    << "#include \"" << targetClassnameHeader << "\"" << std::endl
    << std::endl
    << "// TODO: write the class implementation for " << targetClassname << std::endl
    << std::endl;

  std::cout << sourceCode.str();
  sourceFile << sourceCode.str();
  sourceFile.close();

  std::cout << "Done." << std::endl;
}

void App::printBanner() {
  std::cout
    << APPLICATION_NAME
    << " "
    << "v" << VERSION_MAJOR
    << "." << VERSION_MINOR
    << "." << VERSION_PATCH
    << std::endl
    << LICENSE
    << " license "
    << COPYRIGHT
    << " "
    << AUTHOR
    << " <" << CONTACT << ">"
    << std::endl
    << std::endl;
}

#ifdef TEST
void runUnitTests();
#endif

int main(int argc,char* argv[]) {
  try {
    #ifdef TEST
    runUnitTests();
    #else
    App app(argc, argv);
    #endif
  } catch (std::exception& ex) {
    std::cerr
      << "Runtime Error: "
      << ex.what()
      << std::endl;
  }
  return EXIT_SUCCESS;

  return 0;
}

#ifdef TEST
struct Expector {
  bool operator ()(std::string const& expected) const {
    bool result = buffer.str() == expected;
    if (!result) {
      const char* actual = buffer.str().c_str();
      // because std::cerr is redirected to a buffer,
      // we need to write the the C api error handle
      fprintf(
        stderr,
        "\x1b[31mexpect() failed.\x1b[0m\nExpected: \"%s\"\nActual: \"%s\"\n\n",
        expected.c_str(),
        actual
      );
    } else {
      fprintf(stdout, "*** \x1b[32mOK\x1b[0m\n");
    }
    return result;
  }

  void print() {
    fprintf(stdout, "%s", buffer.str().c_str());
  }

  std::stringstream buffer;
  std::streambuf* cache;
};

class Expect {
  public:
    Expect() {
      output.cache = std::cout.rdbuf(output.buffer.rdbuf());
      error.cache = std::cerr.rdbuf(error.buffer.rdbuf());
    }

    ~Expect() {
      std::cout.rdbuf(output.cache);
      std::cerr.rdbuf(error.cache);
    }

    bool fileExists(std::string const& filename, bool deleteFile = true) {
      std::ifstream inFile(filename);
      bool result = inFile.good();
      inFile.close();
      if (!result) {
        fprintf(
          stderr,
          "\x1b[31mexpect() failed.\x1b[0m\nExpected: \"%s\" exists\nActual: \"%s\" does not exist\n\n",
          filename.c_str(),
          filename.c_str()
        );
      } else {
        fprintf(stdout, "*** \x1b[32mOK\x1b[0m\n");
        if (deleteFile) {
          std::remove(filename.c_str());
        }
      }
      return result;
    }

    Expector output;
    Expector error;
};

void runUnitTests() {
  std::map<std::string, std::function<void(void)>> tests;

  tests["should display version information when passed -v option"] = []() {
    Expect expect;
    int testArgc = 2;
    const char* testArgv[] = {
      "program",
      "-v"
    };
    App app(testArgc, const_cast<char**>(testArgv));
    assert(expect.output("v0.1.0\n"));
  };

  tests["should display version information when passed --version option"] = []() {
    Expect expect;
    int testArgc = 2;
    const char* testArgv[] = {
      "program",
      "--version"
    };
    App app(testArgc, const_cast<char**>(testArgv));
    assert(expect.output("v0.1.0\n"));
  };

  const char* helpText =
  "classgen v0.1.0\n"
  "MIT license (C) Copyright 2018 Richard Marks <ccpsceo@gmail.com>\n"
  "\n"
  "Usage: program ([-h | -v | --help | --version] | {classname})\n"
  "\n"
  "    -h or --help       display this help screen\n"
  "\n"
  "    -v or --version    display the software version\n"
  "\n"
  "    OR\n"
  "\n"
  "    {classname}        path to the name of the class to generate.\n"
  "\n"
  "Examples:\n"
  "\n"
  "Use Case 1 - Generate class files in the current directory\n"
  "    program example\n"
  "\n"
  "This command will create the files \"Example.hpp\" and \"Example.cpp\" in the created directory.\n"
  "\n"
  "Use Case 2 - Generate class files in a nested directory\n"
  "    program relative/path/to/example\n"
  "\n"
  "This command will create in the current directory, the path \"relative/path/to/\" if it does not exist.Next, the files \"Example.hpp\" and \"Example.cpp\" will be created in the created directory.\n"
  "\n";

  tests["should display help information when passed -h option"] = [&helpText]() {
    Expect expect;
    int testArgc = 2;
    const char* testArgv[] = {
      "program",
      "-h"
    };
    App app(testArgc, const_cast<char**>(testArgv));
    assert(expect.output(helpText));
  };

  tests["should generate a header file when passed a filename"] = []() {
    Expect expect;
    int testArgc = 2;
    const char* testArgv[] = {
      "program",
      "test1"
    };
    App app(testArgc, const_cast<char**>(testArgv));
    assert(expect.fileExists("Test1.hpp"));
  };

  tests["should generate a source file when passed a filename"] = []() {
    Expect expect;
    int testArgc = 2;
    const char* testArgv[] = {
      "program",
      "test1"
    };
    App app(testArgc, const_cast<char**>(testArgv));
    assert(expect.fileExists("Test1.cpp"));
  };

  tests["should generate missing directories and a header file when passed a nested path"] = []() {
    Expect expect;
    int testArgc = 2;
    const char* testArgv[] = {
      "program",
      "test2/nested/example"
    };
    App app(testArgc, const_cast<char**>(testArgv));
    assert(expect.fileExists("./test2/nested/Example.hpp"));
    system("rm -r test2");
  };

  tests["should generate missing directories and a source file when passed a nested path"] = []() {
    Expect expect;
    int testArgc = 2;
    const char* testArgv[] = {
      "program",
      "test3/nested/example"
    };
    App app(testArgc, const_cast<char**>(testArgv));
    assert(expect.fileExists("./test3/nested/Example.cpp"));
    system("rm -r test3");
  };

  // run the tests
  for (auto it = tests.begin(); it != tests.end(); ++it) {
    std::cout << "Running Test: \x1b[33;1m" << it->first << "\x1b[0m" << std::endl;
    it->second();
  }
}
#endif
