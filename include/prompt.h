/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prompt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/29 19:40:14 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/16 21:47:25 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H
# define PROMPTS 10
# define PROMPT_DEFAULT 1
# define PROMPT_ALL "Available prompts: turtle, moon, shark, spooky,\
 hell, racoon, cherry\n"

# define TURTLE 1
# define TURTLE1 "\1\x1b[48;5;220m\x1b[30m\2 🐢 \1\x1b[104m\2"
# define TURTLE2 " TurtleShell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"
# define TURTLE3 "\1\x1b[48;5;220m\x1b[30m\2 🐢 \1\x1b[104m\2\
 TurtleShell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m \2\3"

# define MOON 2
# define MOON1 "\1\x1b[48;5;234m\x1b[30m\2 🌙 \1\x1b[38;5;252;48;5;234m\2"
# define MOON2 " Moony Shell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define SHARK 3
# define SHARK1 "\1\x1b[48;5;33m\x1b[30m\2 🦈 \1\x1b[48;5;27;38;5;45m\2"
# define SHARK2 " SharkyShell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define SPOOKY 4
# define SPOOKY1 "\1\x1b[48;5;202m\x1b[30m\2 🎃 \1\x1b[38;5;202;48;5;232m\2"
# define SPOOKY2 " SpookyShell \1\x1b[49m\x1b[38;5;202m❱\2 \1\x1b[0m\2\3"

# define HELL 5
# define HELL1 "\1\x1b[48;5;232m\x1b[30m\2 👹 \1\x1b[101m\2"
# define HELL2 " Mini's hell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define OKAY 6
# define OKAY1 "\1\x1b[48;5;234m\x1b[30m\2 🗿 \1\x1b[38;5;250;48;5;234m\2"
# define OKAY2 " dont care 🗿\1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define RACOON 7
# define RACOON1 "\1\x1b[48;5;252m\x1b[30m\2 🦝 \1\x1b[38;5;231;48;5;233m\2"
# define RACOON2 " RacoonShell \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define GORILA 8
# define GORILA1 "\1\x1b[48;5;237m\x1b[30m\2 🦍 \1\x1b[38;5;160;48;5;237m\2"
# define GORILA2 "  monke    🦍\1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define CHERRY 9
# define CHERRY1 "\1\x1b[48;5;211m\x1b[30m\2 🌸 \1\x1b[38;5;231;48;5;219m\2"
# define CHERRY2 " CherryShell \1\x1b[49m\x1b[38;5;211m❱\2 \1\x1b[0m\2\3"

# define DOOKIE 10
# define DOOKIE1 "\1\x1b[48;5;94m\x1b[30m\2 💩 \1\x1b[38;5;130;48;5;172m\2"
# define DOOKIE2 " DookieShell \1\x1b[49m\x1b[38;5;94m❱\2 \1\x1b[0m\2\3"

# define HD1 "\1\x1b[48;5;235m\x1b[30m\2 📄 \1\x1b[48;5;33;30m\2"
# define HD2 " heredoc \1\x1b[49m\x1b[92m❱\2 \1\x1b[0m\2\3"

# define YELLOW	"\1\33[38;5;220m\2"
# define BLACK	"\1\33[38;5;232m\2"
# define ORANGE	"\1\33[38;5;202m\2"
# define RED	"\1\33[38;5;196m\2"
# define PINK	"\1\33[38;5;198m\2"
# define GREEN	"\1\33[38;5;28m\2"
# define BLUE	"\1\33[38;5;33m\2"
# define L_BLUE	"\1\33[38;5;69m\2"
# define D_BLUE	"\1\33[38;5;21m\2"
# define GREY	"\1\33[38;5;242m\2"
# define PURPLE	"\1\33[38;5;92m\2"
# define VIOLET	"\1\33[38;5;183m\2"
# define RESET	"\1\33[0m\2"
# define END	"\1\33[0m\2\3"

#endif