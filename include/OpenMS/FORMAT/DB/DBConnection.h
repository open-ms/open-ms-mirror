// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2013.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: $
// $Authors: Marc Sturm $
// --------------------------------------------------------------------------

#ifndef OPENMS_FORMAT_DB_DBCONNECTION_H
#define OPENMS_FORMAT_DB_DBCONNECTION_H

#include <OpenMS/config.h>

//QT includes
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

//OpenMS includes
#include <OpenMS/CONCEPT/Exception.h>
#include <OpenMS/DATASTRUCTURES/String.h>
#include <OpenMS/CONCEPT/Types.h>

#include <iostream>

namespace OpenMS
{
  /**
    @brief A class for connecting to a SQL database

    @note Do not use '*' in SELECT statements. The order of result columns is not defined then! Read the QT documentation for details.

    @ingroup DatabaseIO
  */
  class OPENMS_DLLAPI DBConnection
  {
public:
    /**
      @brief Exception in case of an invalid Query



      @ingroup Exceptions
    */
    class OPENMS_DLLAPI InvalidQuery :
      public Exception::BaseException
    {
public:
      InvalidQuery(const char * file, Int line, const char * function, const String & sql_query, const String & sql_error);
      ~InvalidQuery() throw();
    };

    /**
      @brief Exception in case of trying to execute a query without having established a database connection



      @ingroup Exceptions
    */
    class OPENMS_DLLAPI NotConnected :
      public Exception::BaseException
    {
public:
      NotConnected(const char * file, Int line, const char * function);
      ~NotConnected() throw();
    };



    /// Default constructor
    DBConnection();

    ///Destructor
    ~DBConnection();

    /**
      @brief Connects to a SQL database.

        @param db the database name
        @param user the login of the server
        @param password the password for the user
        @param host the host where the server is running (default: "localhost")
        @param port the port where the server is listening (default: 3306)
        @param QTDBDriver the QT database driver used for the connection (default: "QMYSQL", unless you changed it in configure. See "OpenMS/include/OpenMS/config.h")
        @param connection_name Name of the connection (needed for several concurrent connections only)

      @exception InvalidQuery is thrown if the database connection could not be opened
    */
    void connect(const String & db, const String & user, const String & password, const String & host = "localhost", UInt port = 3306, const String & QTDBDriver = DB_PLUGIN, const String & connection_name = "OpenMS_default_connection");

    /// returns if a connection is established.
    bool isConnected() const;

    /**
              @brief disconnects from the SQL database

              @note In order to disconnect, all queries running on a database must be closed. See the QSqlDatabase::removeDatabase(...) documentation.
          */
    void disconnect();

    /**
      @brief Executes a query and returns the result

      The internal pointer of the returned result is positioned @b before the first row.

      @param query an SQL query
      @param first if @em true, the internal pointer of the returned result is positioned to the first record.

              @exception InvalidQuery is thrown if an invalid SQL query was given
              @exception NotConnected if there is no database connection

    */
    QSqlQuery executeQuery(const String & query, bool first = false);

    /**
        @brief Returns a single field of a table as an integer

        The table has to contain an <tt>id</tt> column.

        @param table The table to look the field up
        @param column The column of the table
        @param id The id of the dataset

        @exception InvalidQuery is thrown if an invalid SQL query was given
        @exception NotConnected if there is no database connection
        @exception Exception::ConversionError is thrown if the value could not be converted to the requested type
    */
    Int getIntValue(const String & table, const String & column, const String & id);

    /**
        @brief Returns a single field of a table as a double

        The table has to contain an <tt>id</tt> column.

        @param table The table to look the field up
        @param column The column of the table
        @param id The id of the dataset

        @exception InvalidQuery is thrown if an invalid SQL query was given
        @exception NotConnected if there is no database connection
        @exception Exception::ConversionError is thrown if the value could not be converted to the requested type

    */
    double getDoubleValue(const String & table, const String & column, const String & id);

    /**
        @brief Returns a single field of a table as string

        The table has to contain an <tt>id</tt> column.

        @param table The table to look the field up
        @param column The column of the table
        @param id The id of the dataset

        @exception InvalidQuery is thrown if an invalid SQL query was given
        @exception NotConnected if there is no database connection
        @exception Exception::ConversionError is thrown if the value could not be converted to the requested type
    */
    String getStringValue(const String & table, const String & column, const String & id);

    /**
        @brief Looks up the ID for a specific entry in an table

        If several entries in the table have the desired value in the column, the first one is returned.

        @param table The table to look the field up
        @param column The column of the table
        @param value The value the selected @p column has

        @exception InvalidQuery is thrown if an invalid SQL query was given
        @exception NotConnected if there is no database connection
    */
    UInt getId(const String & table, const String & column, const String & value);

    /// Returns the last auto_increment ID of the SQL database
    UInt getAutoId();

    /// Returns the name of the connected DB
    String DBName() const;

    /**
        @brief Dumps a query result in table format into a stream.

        To dump a result as HTML table, use render(result, cout,"&lt;/td&gt;&lt;td&gt;","&lt;tr&gt;&lt;td&gt;","&lt;/td&gt;&lt;/tr&gt;");

        @param result The result to render
        @param out The output stream to use
        @param separator The string between the fields
        @param line_begin The string at the beginning of each line
        @param line_end The string at the end of each line
    */
    void render(QSqlQuery & result, std::ostream & out = std::cout, const String & separator = " | ", const String & line_begin = "", const String & line_end = "\n");


    /**
        @brief Executes all SQL queries from an container.

        Each line has to be a query or empty.

        @param queries A STL-compliant container of OpenMS String objects

        @exception InvalidQuery is thrown if an invalid SQL query was given
        @exception NotConnected if there is no database connection
    */
    template <class StringListType>
    void executeQueries(const StringListType & queries);

private:

    /// Name (handle) of the connection
    QString connection_name_;

    /// Returns the current database connection defined by connection_name_
    inline QSqlDatabase getDB_() const
    {
      return QSqlDatabase::database(connection_name_, false);
    }

  };



  //---------------------------------------------------------------
  //  Implementation of the inline / template functions
  //---------------------------------------------------------------

  template <class StringListType>
  void DBConnection::executeQueries(const StringListType & queries)
  {
    String line;
    for (typename StringListType::const_iterator it = queries.begin(); it != queries.end(); ++it)
    {
      line = *it;
      line.trim();
      if (line != "")
      {
        executeQuery(line);
      }
    }
  }

}

#endif
