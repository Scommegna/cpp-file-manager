# File Manager em C++ (POSIX)

Este projeto consiste na implementação de um **File Manager em linha de comando (CLI)** utilizando **C++ e a API POSIX**, com foco em **operações de baixo nível sobre o sistema de arquivos**.

O projeto faz parte de um Trabalho de Conclusão de Curso (TCC), cujo objetivo é **comparar diferentes aspectos entre implementações em C++ e Rust**, incluindo desempenho, segurança e uso de recursos.

---

# Objetivo

Desenvolver uma aplicação de gerenciamento de arquivos que permita:

- interação direta com o sistema operacional
- uso de chamadas POSIX (baixo nível)
- medição de métricas relevantes

E posteriormente:

> comparar essa implementação com uma versão equivalente em Rust.

---

# Tecnologias utilizadas

- C++17
- API POSIX
- Linux
- std::thread (multithreading)

---

# API utilizada

O projeto utiliza a API POSIX para manipulação de arquivos e diretórios:

- opendir, readdir, closedir
- stat, lstat
- open, read, write, close
- unlink, rmdir, rename
- chdir, getcwd

---

# Funcionalidades

- fm list <dir>  
- fm cd <dir>  
- fm pwd  
- fm info <file>  
- fm copy <src> <dst>  
- fm move <src> <dst>  
- fm delete <file>  
- fm search <name> <dir>  
- fm search-mt <name> <dir> <threads>  
- fm tree <dir>  

---

# Benchmark e Métricas

Use:

--benchmark ou -b

Exemplo:

fm search main . --benchmark

Saída:

[Benchmark]  
Execution time: 12 ms  
Resident memory: 5460 KB  

---

# Métricas analisadas no TCC

## Tempo de execução
Medido com std::chrono.

## Uso de memória
Obtido via /proc/self/status (VmRSS).

## Performance de I/O
Avaliada em copy, search e tree.

## Busca concorrente
Comparação entre single-thread e multithread.

## Segurança de memória
Comparação entre C++ e Rust.

## Complexidade
Avaliação qualitativa da implementação.

---

# Busca Multithread

fila de diretórios  
↓  
threads processam diretórios  
↓  
novos diretórios são adicionados  
↓  
resultados são impressos  

---

# Segurança

- uso de lstat()
- controle de concorrência com mutex

---

# Uso no TCC

Este projeto será utilizado para:

- comparação C++ vs Rust
- análise de performance
- estudo de concorrência
- avaliação de memória

---

# Referências

https://pubs.opengroup.org/onlinepubs/9699919799/  
https://man7.org/linux/man-pages/  
https://www.geeksforgeeks.org/cpp/file-management-system-in-cpp/  
https://github.com/krshrimali/CPP-File-Manager  

---

# Autor

Lucas Scommegna
