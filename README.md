# MinMaxNumber
C++ Program that returns the smallest and biggest number in an SQL Column

**WARNING: THIS PROGRAM IS NOT INTENDED TO BE USED IN A WORK/PRODUCTION ENVIROMENT, THERE ARE BASIC FUNCTIONS IN MYSQL TO ACCOMPLISE THE THINGS THIS PROGRAM IS DOING. PASSWORDS ARE PASSED TO THE PROGRAM IN COMPLETELY UNPROTECTED PLAIN TEXT MAKING IT VERY INSECURE. THIS APPLICATION WAS INTENDED AS A LEARNING EXPERIENCE FOR ME ONLY, I AM NOT RESPONSABLE FOR ANY DAMAGES IF YOU DO USE THIS PROGRAM**


## Documentation
### Application Goals
1. Parse command line arguments given in any order using either --long-form or -s (short form) ✔
    1. Show error if not enough arguments have been entered or if unknown arguments have been entered ✔
    2. Create a help page that can be called by the user in order to learn what arguments need to be given ✔
2. Connect to an MySQL database using the details obtained from the command line ✔
3. Fetch all the numbers in the specified column ✔
4. Perform calculations about the numbers in the column ✔
   1. The smallest number in the column ✔
   2. The biggest number in the column ✔
   3. The total sum of all the numbers in the column ✔
   4. The average of all the numbers in the column ✔
   5. Display the results ✔
5. Match the number displayed with a name if applicable ❌
6. Read MySQL connection details from a config file ❌


### Structure of the program
- MinMaxNumber.cpp - This is the main file of the program, it contains code to parse command line arguments and is the central file from which functions inside ofther files are called. This file will be refered to as "main" from now on.
- SQLHandler.cpp - This file contains functions to connect and communicate with the target MySQL server.
- SQLHandler.hpp - This is just a header file for SQLHandler.cpp to allow main to call it's functions.
- w32_colour.hpp - This is a header file I created for changing background and foreground console colours on Windows, this is used when I need to display a warning or error message.


### How the goals were achived
#### 1.
`#include "cxxopts.hpp"`

For this I used the [cxxopts](https://github.com/jarro2783/cxxopts) header file which provides functions to parse command line arguments as any data type you want.

`std::string url, user, pass, db, table, column;`

First of all we initalize 6 string variables. We will use these later on to store the results from the command line.

```cpp
options.add_options()
    ("h,help", "Show help page")
    ("a,address", "MySQl server address", cxxopts::value<std::string>())
    ("u,user", "MySQL server username", cxxopts::value<std::string>())
    ("p,password", "MySQL user password", cxxopts::value<std::string>())
    ("d,database", "MySQL database name", cxxopts::value<std::string>())
    ("t,table", "MySQL table name", cxxopts::value<std::string>())
    ("c,column", "MySQL table column name", cxxopts::value<std::string>());
auto result = options.parse(argc, argv);
```
Next we need to tell cxxopts what arguments we want to look for on the command line and what data type to store them as. Whether or not to show the help page will be stored as a boolean (the defualt), the rest are strings. We also need to create a variable to store the result of cxxopts parsing the data we gave it.
```cpp
url     = result["address"].as<std::string>();
user    = result["user"].as<std::string>();
pass    = result["password"].as<std::string>();
db      = result["database"].as<std::string>();
table   = result["table"].as<std::string>();
column  = result["column"].as<std::string>();
```
Finally we assign the strings we created earlier to the values cxxopts obtained from the command line.

#### 1 (i).
For this we simply checked the size of argc (an interger provided by C++ telling you how many argument have benn passed to the program). If it is less than the required amount of arguments and the user has not requested to see the help page, an error is shown.

<img src="https://github.com/RolandWH/MinMaxNumber/raw/master/img/not_enough_arguments?raw=true"/>

```cpp
} catch (const cxxopts::OptionException& e) {
    std::cout << "Error parsing options: " << e.what() << std::endl;
    return EXIT_FAILURE;
}
```
If cxxopts encounters an unknown argument an error will be thrown telling the user what the offending argument is.

<img src="https://github.com/RolandWH/MinMaxNumber/raw/master/img/incorrect_argument?raw=true"/>

#### 1 (ii).
```cpp
if (result.count("help")) {
    // Print help page line by line
    return EXIT_SUCSESS;
}
```
We use a cxxopts function to check whether the help page was requested by the user, if it was we print it and exit.

<img src="https://github.com/RolandWH/MinMaxNumber/raw/master/img/help_page?raw=true"/>

#### 2.
For this part of the program we switch to using the seperate *SQLHandler.cpp* file. It contains two functions: one for connecting to the database and another for returning the numbers contained in a specified column.

`#include <mysql/jdbc.h>`

For connecting to the database I am using the [mysql-connector-cpp](https://github.com/mysql/mysql-connector-cpp) library. People who are familiar with this library might notice that I am using the legacy header file and not the more modern one, this is simply because there was much more documentation available for the legacy one, besides this program is performing such a simple SQL task that it does not particually matter what implementation is used.

```cpp
// main file
if (!SQLConnect(url, user, pass, db)) {
    return EXIT_FAILURE;
}
```
Back in the main file we call the *SQLConnect* function, which is a boolean function that attempts to connect to the SQL database. It will either return true or false depending on whether the connection was sucsessful or not. If there is an error connecting to the database an error will be shown. There are many different types of errors that could be displayed, pictured below is an error you get if an incorrect database name is provided. The other errors take the same format but with different descriptions and error codes.

<img src="https://github.com/RolandWH/MinMaxNumber/raw/master/img/incorrect_databse?raw=true"/>

#### 3.
`std::vector<int> numList = FetchColumns(table, column);`

Back in main we create a vector to store the list of numbers returned by SQLHandler function `FetchColumns`. The numbers in the provided column is accessed by issuing the SQL statment: `SELECT column FROM table`, where column and table are replaced with the actual names. This query is issued by the `executeQuery` MySQL Connector function. Once all the numbers in the specified column have been recorded they are returned to main.

#### 4 (i, ii).
```cpp
int smallest = INT_MAX;
int biggest = NULL;
for (int i = 0; i < numList.size(); i++)
{
    smallest = std::min(smallest, numList[i]);
    biggest = std::max(biggest, numList[i]);
}
```
To find the smallest and biggest numbers in the column we make use of the built in C++ functions `min` and `max`. Counterintuitively, we initally set the `smallest` variable to the biggest it can possibly be and `biggest` variable to the smallest it can possibly be. This is so that the smallest number in our actual list of numbers will definatally be smaller than the inital value of `smallest` and that the biggest number will always be bigger than the inital value of `biggest`. Next we loop through the numbers in our list compairing the current number to the previous smallest and biggest numbers. By the end of this looping we will be left with the smallest and biggest numbers in our column.

#### 4 (iii, iv).
```cpp
int sum = std::accumulate(numList.begin(), numList.end(), 0);
int avg = sum / numList.size();
```

For this we use the C++ accumulate function which will sum up all the numbers in our list and return the result into our `sum` integer. Finally we find the average by dividing the sum of our numbers by the size of the list.

#### 4 (v).
First of all, if all our previous calculations are zero then we display a warning about it, it may be that a column that does not contain integers has been provided by mistake. One by one we output the results of the calculations we performed earlier with an accompanying description. The output pictured below is what you recive when you run the program on the following table of Countries using the `Population` column:

| Country Name   | Population    |
| -------------- | ------------- |
| United Kingdom | 67,530,172    |
| United States  | 329,064,917   |
| China          | 1,433,783,686 |
| Japan          | 126,860,301   |
| Russia         | 145,872,256   |

<img src="https://github.com/RolandWH/MinMaxNumber/raw/master/img/population_output?raw=true"/>

