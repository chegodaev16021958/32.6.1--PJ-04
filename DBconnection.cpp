#include "DBconnection.h"

bool DBconnection::open()
{
    // Выделяем дескриптор для базы данных
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
        return true;

    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        return true;

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
        return true;

    // Устанавливаем соединение с сервером  
    switch (SQLDriverConnect(sqlConnHandle,
        GetDesktopWindow(),
        (SQLWCHAR*)L"DRIVER={MySQL ODBC 8.1 ANSI Driver};SERVER=localhost;PORT=3306;DATABASE=7tchat;UID=root;PWD=Pass4root$;",
        SQL_NTS,
        retconstring,
        1024,
        NULL,
        SQL_DRIVER_COMPLETE))
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        std::cout << "Successfully connected to SQL Server\n";
        //return false;
        break;
    case SQL_INVALID_HANDLE:
    case SQL_ERROR:
        std::cout << "Could not connect to SQL Server\n";
        //return true;
        break;
    default:
        break;
    }

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        return true;
    else return false;
}

void DBconnection::close()
{
    // Закрываем соединение
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
}

SQLRETURN DBconnection::execQuery(SQLWCHAR* StatementText, SQLINTEGER TextLength)
{
    return SQLExecDirect(hstmt, StatementText, TextLength);
}

bool DBconnection::execTestQuery()
{
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT @@VERSION", SQL_NTS))
    {
        std::cout << "Error querying SQL Server \n";
        return false;
    }
    else
    {
        //Объявление структуры данных для результата запроса версии SQL
        SQLCHAR sqlVersion[SQL_RESULT_LEN];
        SQLLEN ptrSqlVersion;

        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
        {

            SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);
            std::cout << "\nQuery Result: ";
            std::cout << sqlVersion << std::endl;
        }
        return false;
    }
}