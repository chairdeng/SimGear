/**
 * \file strutils.hxx
 * String utilities.
 */

// Written by Bernie Bright, started 1998
//
// Copyright (C) 1998  Bernie Bright - bbright@bigpond.net.au
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// $Id$


#ifndef STRUTILS_H
#define STRUTILS_H

#include <simgear/compiler.h>

#include <string>
#include <vector>
#include <type_traits>
#include <cstdlib>

typedef std::vector < std::string > string_list;

namespace simgear {
  namespace strutils {

	/**
	 * utf8ToLatin1() convert utf8 to latin, useful for accent character (i.e éâàîè...)
	 */
	std::string utf8ToLatin1( std::string & s_utf8 );

// 	/** 
// 	 * atof() wrapper for "string" type
// 	 */
// 	inline double
// 	atof( const string& str )
// 	{
// 	    return ::atof( str.c_str() );
// 	}

// 	/**
// 	 * atoi() wrapper for "string" type
// 	 */
// 	inline int
// 	atoi( const string& str )
// 	{
// 	    return ::atoi( str.c_str() );
// 	}

	/**
	 * Strip leading and/or trailing whitespace from s.
	 * @param s String to strip.
	 * @return The stripped string.
	 */
	std::string lstrip( const std::string& s );
	std::string rstrip( const std::string& s );
	std::string strip( const std::string& s );

	/**
         * Return a new string with any trailing \r and \n characters removed.
         * Typically useful to clean a CR-terminated line obtained from
         * std::getline() which, upon reading CRLF (\r\n), discards the Line
         * Feed character (\n) but leaves the Carriage Return (\r) in the
         * string.
         * @param s Input string
         * @return The cleaned string
         */
        std::string stripTrailingNewlines(const std::string& s);

	/**
         * Strip any trailing \r and \n characters from a string.
         * Should have slightly less overhead than stripTrailingNewlines().
         * @param s Input string (modified in-place)
         */
        void stripTrailingNewlines_inplace(std::string& s);

        /**
         * Right-padding of a string to a given length
         * @param s String to pad
         * @param length The total length of the resulting string
         * @param c The character to pad with
         * @return The padded string
         */
        std::string rpad( const std::string & s, size_t length, char c );

        /**
         * Left-padding of a string to a given length
         * @param s String to pad
         * @param length The total length of the resulting string
         * @param c The character to pad with
         * @return The padded string
         */
        std::string lpad( const std::string & s, size_t length, char c );

	/**
	 * Split a string into a words using 'sep' as the delimiter string.
	 * Produces a result similar to the perl and python functions of the
	 * same name.
	 * 
	 * @param s The string to split into words,
	 * @param sep Word delimiters.  If not specified then any whitespace is a separator,
	 * @param maxsplit If given, splits at no more than maxsplit places,
	 * resulting in at most maxsplit+1 words.
	 * @return Array of words.
	 */
	string_list
	split( const std::string& s,
	       const char* sep = 0,
	       int maxsplit = 0 );

      /**
       * split a string on any of several characters. Commonly used to deal
       * with strings containing whitespace, newlines. To parse CSS style
       * string, use with '\n\t ,' as the seperator list.
       *
       * Note consecutive seperators will not produce empty entries in the
       * the result, i.e splitting 'a,b,,c,d' with a ',' will produce a result
       * with four entries, not five.
       */
      string_list split_on_any_of(const std::string&, const char* seperators);

       /**
        * create a single string by joining the elements of a list with
        * another string.
        */
       std::string join(const string_list& l, const std::string& joinWith = "");

	/**
	 * Test if a string starts with a string 
	 *
	 * @param s The string to be tested
	 * @param substr The string to test
	 * @return True, if s starts with substr, False otherwise
	 */
	bool starts_with( const std::string & s, const std::string & substr );

	/**
	 * Test if a string ends with a string 
	 *
	 * @param s The string to be tested
	 * @param substr The string to test
	 * @return True, if s ends with substr, False otherwise
	 */
	bool ends_with( const std::string & s, const std::string & substr );
  
    /**
     * Strip all leading/trailing whitespace, and transform all interal
     * whitespace into a single ' ' character - i.e newlines/carriage returns/
     * tabs/multiple spaces will be condensed.
     */
    std::string simplify(const std::string& s);
    
    /**
     * convert a string representing a decimal number, to an int
     */
    int to_int(const std::string& s, int base = 10);

   /** Convert a char to the integer it represents in the specified BASE.
     *
     * Contrary to std::isdigit() and std::isxdigit(), only the standard ASCII
     * digits for BASE are accepted (with both uppercase and lowercase 'a'-'f'
     * letters for base 16). Throw sg_range_exception if the char is not a
     * valid digit for this base.
     *
     * See template specializations in strutils.cxx.
     */
    template<int BASE>
    int digitValue(char c);

    /** Return:
     *   - std::string("decimal") if BASE is 10;
     *   - std::string("hexadecimal") if BASE is 16.
     *
     * Template specializations in strutils.cxx.
     */
    template<int BASE>
    std::string numerationBaseAdjective();

    /** Convert a string representing an integer to an integral type.
     *
     * The input string must be non-empty and contain only digits of the
     * specified BASE (template parameter). Throw:
     *   - sg_format_exception if the input string doesn't respect these
     *     constraints;
     *   - sg_range_exception if the value can't be represented by type T
     *     (i.e., if it is too large).
     *
     * Explicit template instantiations are added as needed in strutils.cxx.
     * Have a look there and enable the ones you need!
     */
    template<
        class T,
        int BASE = 10,
        typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    T readNonNegativeInt(const std::string& s);

    /**
     * Convert a string representing a boolean, to a bool.
     * Accepted values include YES, true, 0, 1, false, no, True,
     */
    bool to_bool(const std::string& s);

    /**
     * Like strcmp(), but for dotted versions strings NN.NN.NN
     * any number of terms are supported.
     * @return 0 if versions match, -ve number if v1 is lower, +ve if v1
     * is greater
     * @param maxComponents is the maximum number of components to look at.
     * This can be used to ignore (say) the patch level by setting it to 2
     */
    int compare_versions(const std::string& v1, const std::string& v2, int maxComponents = 0);

    /**
     * Convert a string to upper case.
     * @return upper case string
     */
    std::string uppercase(const std::string &s);

   /**
    * Convert a string to lower case.
    * @return lower case string
    */
   std::string lowercase(const std::string &s);
   
  /**
   * Convert a string to lower case in place
   */
  void lowercase(std::string &s);
  
	/**
     * convert a string in the local Windows 8-bit encoding to UTF-8
     * (no-op on other platforms)
     */
    std::string convertWindowsLocal8BitToUtf8(const std::string& a);

      /**
       *
       */
      std::string convertUtf8ToWindowsLocal8Bit(const std::string& a);

    std::wstring convertUtf8ToWString(const std::string& a);
	std::string convertWStringToUtf8(const std::wstring& w);

    /**
     * Get md5 hash of raw data.
     */
    std::string md5(const unsigned char* data, size_t num);
    std::string md5(const char* data, size_t num);
    std::string md5(const std::string& str);

    /**
     * convert base-64 encoded data to raw bytes (possibly with embedded
     * NULs). Throws an exception if input data is not base64, or is
     * malformed
     */
      void decodeBase64(const std::string& a, std::vector<unsigned char>& output);

    /**
     * convert bytes to hexadecimal equivalent
     */
    std::string encodeHex(const std::string& bytes);

    std::string encodeHex(const unsigned char* rawBytes, unsigned int length);

    /**
     * Backslash-escape a string for C/C++ string literal syntax.
     *
     * @param s Input string.
     * @return a copy of the input string with proper escaping, so that if the
     *         result is part of a C or C++ file and enclosed in double
     *         quotes, it can be used to represent a string literal that is
     *         equal to the input string.
     *
     * @note For every std::string s: unescape(escape(s)) == s
     * @see unescape()
     */
    std::string escape(const std::string& s);

    /**
     * Unescape string.
     *
     * @param str String possibly containing escaped characters.
     * @return string with escaped characters replaced by single character
     *         values.
     *
     * @note For every std::string s: unescape(escape(s)) == s
     * @see escape()
     */
    std::string unescape(const char* str);

    inline std::string unescape(const std::string& str)
    { return unescape(str.c_str()); }

      /**
       * Check a printf-style format string for dangerous (buffer-overflowing,
       * memory re-writing) format tokens. If a problematic token is
       * found, logs an error (SG_WARN) and returns an empty format string.
       */
      std::string sanitizePrintfFormat(const std::string& input);

    /**
     * Get the message corresponding to a given value of errno.
     *
     * Similar to strerror(), except it should be thread-safe and returns an
     * std::string.
     */
    std::string error_string(int errnum);


      /**
       * Match a property path, obtained from prop->getPath(), against a
       * template string. Templates are allowed to contain widlcards denoted by
       * an asterix in certain places - at the end of names, or inside indices.
       * Note that paths returned by getPath() always include an index on every
       * path component, so template strings should be structured accordingly.
       *
       * Examples: 
       *   /foo[*]/bar* will match /foo/barber, /foo[2]/bargain
       *   /views[0]/view[*]/f* will match /views[0]/view[99]/foo,
       *      /views[0]/view[4]/fig, /views[0]/view[1000]/flight
       */
      bool matchPropPathToTemplate(const std::string& path, const std::string& templatePath);
  } // end namespace strutils
} // end namespace simgear

#endif // STRUTILS_H

