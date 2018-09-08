///@file: enum_temp.h
#pragma once

/// The kinds of files we read and write, in order of preference:
namespace my_ns {
    typedef enum {
        FIRST_FILE_FORMAT,
        MsgPackFile = FIRST_FILE_FORMAT,
        BinaryFile,
        JsonFile,
        NUM_FILE_FORMATS,
        UNKNOWN     = NUM_FILE_FORMATS
    }   FileFormat;

    /// Please keep the FileFormatExts parallel to FileFormat enum values.
    extern const char * const FileFormatExts[];
}   // namespace my_ns
