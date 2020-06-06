from game import Game, FIRST_PLAYER, SECOND_PLAYER


if __name__ == '__main__':
    game = Game()
    my_player = int(input()) + 1
    op_player = SECOND_PLAYER if my_player == FIRST_PLAYER else FIRST_PLAYER

    if my_player == FIRST_PLAYER:
        game.build_quoridor_mode(my_player)

    server_message = input()
    while not server_message == 'over':
        op_move = server_message.split(' ')

        if op_move[0] == 'move':
            game.move_player(op_player, int(op_move[1]), int(op_move[2]))
        else:
            game.build_wall(op_player, int(op_move[1]), int(op_move[2]), int(op_move[3]), int(op_move[4]))

        game.build_quoridor_mode(my_player)
        server_message = input()
