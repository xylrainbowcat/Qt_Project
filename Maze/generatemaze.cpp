#include "gamewindow.h"
#include "ui_gamewindow.h"

void GameWindow::generate(int row, int col) {
    srand((unsigned)time(0));

//    for (int i = 0; i < len; i++) {
//        maze[i][0] = 1;
//        maze[0][i] = 1;
//        maze[len - 1][i] = 1;
//        maze[i][len - 1] = 1;
//    }

    memset(maze, 0, sizeof maze);
    //墙队列，包括X , Y
    QVector<QPair<int, int>> wall;

    //任取初始值
    wall.push_back({0, 0});

    //当墙队列为空时结束循环
    while (wall.size()) {
        //在墙队列中随机取一点
        int random = rand() % wall.size();
        int r = wall[random].first;
        int c = wall[random].second;

        //判读上下左右四个方向是否为路
        int cnt = 0;
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < row && nc >= 0 && nc < col && maze[nr][nc]) {
                ++cnt;
            }
        }


        if (cnt <= 1) {
            maze[r][c] = 1;
            //在墙队列中插入新的墙
            for (int i = 0; i < 4; ++i) {
                int nr = r + dr[i];
                int nc = c + dc[i];
                if (nr >= 0 && nr < row && nc >= 0 && nc < col && !maze[nr][nc]) {
                    wall.push_back({nr, nc});
                }
            }
        }

        //删除当前墙
        wall.erase(wall.begin() + random);
    }

    //设置迷宫进出口
    //maze[1][0] = 1;
//    for (int i = len - 3; i >= 0; i--) {
//        if (maze[i][len - 3] == 1) {
//            maze[i][len - 2] = 1;
//            break;
//        }
//    }
}
