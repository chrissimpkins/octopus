/* main.h
*  Copyright 2013 Christopher Simpkins.  MIT License
*/
#ifndef MAIN_H
#define MAIN_H

inline void show_help();
inline void show_usage();
inline void show_version();
inline std::vector<std::string> create_CL_vector(int argc, char const *arg[]);
inline const std::string currentDateTime();

#endif