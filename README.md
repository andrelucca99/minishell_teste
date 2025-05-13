# ✅ Semana 1 — Estrutura e Execução Básica
### 🔹 Parsing e Tokens
 - Estrutura de tokens t_token e comandos t_command

 - Parser que transforma uma lista de tokens em comandos com:

 	- Argumentos (args)

	- Redirecionamento de entrada (<)

	- Redirecionamento de saída (>, >>)

	- Heredoc (<<) reservado

- Substituição do uso de calloc por malloc com inicialização manual

### 🔹 Execução simples
- Função execute_commands(t_command *cmds) com suporte a:

	- Comando simples (ls, echo)

	- Redirecionamento de entrada e saída (<, >, >>)

	- Execução de comandos externos com execve

	- Cálculo manual de PATH com getenv("PATH") + strtok

### 🔹 Built-ins
- Função exec_builtin(char **args) com suporte a:

	- echo

	- pwd

	- env

	- cd

	- exit

- Separação de cada built-in em função própria

### ✅ Melhorias de gerenciamento de memória
- Criação da função free_commands(t_command *cmds)

- Criação da função free_tokens(t_token *tokens)

- Correção de vazamentos causados por ponteiros não duplicados (argv[i] = strdup)

- Liberação de variáveis temporárias no parser (argv[])

- Manutenção de argv[argc] = NULL sempre que necessário

## ✅ Semana 2 — Integração e Finalização
- 🔹 Execução com múltiplos pipes
Execução encadeada com cmd1 | cmd2 | cmd3 | ...

- Controle de fd_in e fd[2] para gerenciamento de entradas/saídas

- Redirecionamentos aplicados corretamente com pipes

### 🔹 Correção de execução de built-ins no pai
- Built-ins como cd e exit são executados no processo pai se:

	- Não houver pipes

	- Não houver redirecionamento

	- For comando único

### 🔹 Tratamento de sinais
- Ctrl+C (SIGINT)

	- Interrompe input com linha limpa e prompt restaurado

- Ctrl+D (EOF)

	- Finaliza a shell com mensagem exit

- Ctrl+\ (SIGQUIT)

	- Ignorado no shell principal

	- Restaurado no processo filho

	- Shell imprime "Quit (core dumped)" quando necessário

### 🔹 Preparações para testes
- Criação de função handle_sigint

- Atualização do main com:

	- Instalação de handlers

	- Liberação adequada de memória

	- Integração completa com parser, executor e signal handling