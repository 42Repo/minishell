# Project Name
Minishell Project

TODO [] - Finir la doc doxygen dans le .h
TODO [] - Fix max 16 heredoc
TODO [] - valgrind error 
TODO [] - Norm All
TODO [] - Changer la logique des execution_pipe / execution 
TODO [] - Here_doc changer les signaux pour le ctrl+c et le ctrl+d a fix
TODO [] - gerer le + sur export +=
TODO [] - Enlever l'expander sur le heredoc si on a : << $USER l'eof est $USER on le fait jamais dans les eof
TODO [] - Erreur sur export
                ```
                export tmp_test="/bin/echo 42"
                $tmp_test
                $tmp_test 42
                export tmp_test="/bin/echo"
                $tmp_test 42 42
                ```
                echo $tmp_test -> _test
TODO [] - fix echo -n
TODO [] - fix exit max 64 bit