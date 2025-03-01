# 🐚 Minishell

Minishell is a simple implementation of a terminal shell, allowing you to execute commands, use pipes, and navigate directories. It supports running system programs and implements basic shell functionalities.


## Compilation and Execution

To compile **Minishell**, simply run:

```sh
make
```

This will generate the **executable** file `minishell`.  
Now, to start the shell, run:

```sh
./minishell
```


## Features Implemented

✅ **Execution of system commands**  
✅ **Input and output redirection (`<`, `>`, `>>`)**  
✅ **Pipe support (`|`)**   
✅ **Built-in commands (`cd`, `exit`, `pwd`, `echo`)**  


## Using Minishell

You can use system commands like:

```sh
ls -l
```

Or provide the absolute path of the command:

```sh
/bin/ls -l
```

### 🔹 **Using Pipes (`|`)**
Minishell supports **chaining commands** using pipes:

```sh
cat README.md | grep shell
```

### 🔹 **Input and Output Redirection**
Redirecting the output of a command to a file:

```sh
ls > file_list.txt
```

Reading input from a file:

```sh
cat < file_list.txt
```

Appending output to a file without overwriting:

```sh
echo "New line" >> file.txt
```

## Built-in Commands

Minishell also implements some **built-in commands**, which do not require `execve()`:

| **Command** | **Description** |
|------------|--------------|
| `cd <directory>` | Changes to the specified directory |
| `pwd` | Displays the current directory |
| `echo <text>` | Prints the text to the screen |
| `exit` | Closes Minishell |


