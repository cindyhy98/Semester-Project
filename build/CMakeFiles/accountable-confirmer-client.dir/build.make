# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build

# Include any dependencies generated for this target.
include CMakeFiles/accountable-confirmer-client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/accountable-confirmer-client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/accountable-confirmer-client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/accountable-confirmer-client.dir/flags.make

CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.o: CMakeFiles/accountable-confirmer-client.dir/flags.make
CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.o: ../src/ac.cpp
CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.o: CMakeFiles/accountable-confirmer-client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.o -MF CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.o.d -o CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.o -c /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/ac.cpp

CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/ac.cpp > CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.i

CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/ac.cpp -o CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.s

CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.o: CMakeFiles/accountable-confirmer-client.dir/flags.make
CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.o: ../src/ac_bls.cpp
CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.o: CMakeFiles/accountable-confirmer-client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.o -MF CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.o.d -o CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.o -c /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/ac_bls.cpp

CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/ac_bls.cpp > CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.i

CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/ac_bls.cpp -o CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.s

CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.o: CMakeFiles/accountable-confirmer-client.dir/flags.make
CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.o: ../src/ecdsa_pki.cpp
CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.o: CMakeFiles/accountable-confirmer-client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.o -MF CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.o.d -o CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.o -c /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/ecdsa_pki.cpp

CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/ecdsa_pki.cpp > CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.i

CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/ecdsa_pki.cpp -o CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.s

CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.o: CMakeFiles/accountable-confirmer-client.dir/flags.make
CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.o: ../src/message.cpp
CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.o: CMakeFiles/accountable-confirmer-client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.o -MF CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.o.d -o CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.o -c /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/message.cpp

CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/message.cpp > CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.i

CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/message.cpp -o CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.s

CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.o: CMakeFiles/accountable-confirmer-client.dir/flags.make
CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.o: ../src/socket.cpp
CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.o: CMakeFiles/accountable-confirmer-client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.o -MF CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.o.d -o CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.o -c /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/socket.cpp

CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/socket.cpp > CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.i

CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/src/socket.cpp -o CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.s

CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.o: CMakeFiles/accountable-confirmer-client.dir/flags.make
CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.o: ../main_client.cpp
CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.o: CMakeFiles/accountable-confirmer-client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.o -MF CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.o.d -o CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.o -c /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/main_client.cpp

CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/main_client.cpp > CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.i

CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/main_client.cpp -o CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.s

# Object files for target accountable-confirmer-client
accountable__confirmer__client_OBJECTS = \
"CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.o" \
"CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.o" \
"CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.o" \
"CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.o" \
"CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.o" \
"CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.o"

# External object files for target accountable-confirmer-client
accountable__confirmer__client_EXTERNAL_OBJECTS =

accountable-confirmer-client: CMakeFiles/accountable-confirmer-client.dir/src/ac.cpp.o
accountable-confirmer-client: CMakeFiles/accountable-confirmer-client.dir/src/ac_bls.cpp.o
accountable-confirmer-client: CMakeFiles/accountable-confirmer-client.dir/src/ecdsa_pki.cpp.o
accountable-confirmer-client: CMakeFiles/accountable-confirmer-client.dir/src/message.cpp.o
accountable-confirmer-client: CMakeFiles/accountable-confirmer-client.dir/src/socket.cpp.o
accountable-confirmer-client: CMakeFiles/accountable-confirmer-client.dir/main_client.cpp.o
accountable-confirmer-client: CMakeFiles/accountable-confirmer-client.dir/build.make
accountable-confirmer-client: /usr/local/opt/openssl@1.1/lib/libcrypto.a
accountable-confirmer-client: ../secp256k1/.libs/libsecp256k1.a
accountable-confirmer-client: /usr/local/lib/libbls384_256.dylib
accountable-confirmer-client: CMakeFiles/accountable-confirmer-client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable accountable-confirmer-client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/accountable-confirmer-client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/accountable-confirmer-client.dir/build: accountable-confirmer-client
.PHONY : CMakeFiles/accountable-confirmer-client.dir/build

CMakeFiles/accountable-confirmer-client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/accountable-confirmer-client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/accountable-confirmer-client.dir/clean

CMakeFiles/accountable-confirmer-client.dir/depend:
	cd /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build /Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/CMakeFiles/accountable-confirmer-client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/accountable-confirmer-client.dir/depend
