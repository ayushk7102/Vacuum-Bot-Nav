/*
#include <opencv/cv.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
*/
#include <algorithm>
#include <iostream>
#include <list>
#include <opencv2/opencv.hpp>
#include <queue>
#include <math.h>
#include "a_star.h"

using namespace cv;
using namespace std;

typedef pair<int, int> Pair;

class dPoint
{
public:
    int x;
    int y;
dPoint()
{
    x=0;
    y=0;
}
dPoint(int x, int y)
    {
        this->x = x;
        this->y = y;

    }
void print_pt()
{
    cout << "("+to_string(x)+","+to_string(y)+")" << endl;
}
string getstr()
    {
       return "("+to_string(x)+","+to_string(y)+")" ;
    }
};

Pair makepair(dPoint p)
{
    Pair pa(p.y, p.x);
    return pa;


}

dPoint makepoint(Pair pa)
{
    dPoint dp(pa.second, pa.first);
    return dp;
}
class Robot
{
public:
    int x;
    int y;
    dPoint p;
    int dir; //DIRECTION: 0 (UP), 1(RIGHT), 2(DOWN), 3(LEFT)

    Robot()
    {
        x = 0;
        y = 0;
        dir = 0;
    }
    Robot(int x, int y)
    {
        this->x = x;
        this->y = y;

        p.x = x;
        p.y = y;

        dir = 0; //INITIALISED UPWARDS
    }

    int getx()
    {
        return x;
    }
    int gety()
    {
        return y;
    }
    void set_xy(int xa, int ya)
    {
        this->x = xa;
        this->y = ya;


    }

    void setdir(int d)
    {
        dir = d;

    }

    void turn(int rotations)
    {
        if(rotations < 0)
            {
                rotations += 4;
            }

        for(int i=1; i<=rotations; i++)
            {
                if(dir == 3)
                    {
                        dir = 0;
                    }
                dir++;

            }
    }

    void move_bot(int steps)
    {
        switch(dir)
        {
        case 0:
            y-=steps;
            break;
        case 1:
            x+=steps;
            break;
        case 2:
            y+=steps;
            break;
        case 3:
            x-=steps;
            break;

        }

    }


    string printloc()
    {
        //cout << "i'm here" << endl;
        cout << to_string(x)+","+to_string(y) << endl;
        return "";
    }

    dPoint getpoint()
    {
        dPoint p(x, y);
        return p;
    }


};
dPoint get(list<dPoint> _list, int _i){
    list<dPoint>::iterator it = _list.begin();
    for(int i=0; i<_list.size(); i++){

        ++it;
    }
    return *it;
}

float distance(dPoint p1, dPoint p2)
{
    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;

    return sqrt(pow((x2- x1), 2) + pow((y2 - y1), 2));
}


int MAP_SIZE = 64;

int grid[64][64]= {};
list<dPoint> plist ;

Mat image;
Mat map_img;
string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
void printlist(list<dPoint> pist)
{
    list<dPoint>::iterator it = pist.begin();
    cout << "[";
    for(int i=0; i<pist.size(); i++){

        //cout <<"\r"<< (*it).getstr();
        cout << (*it).getstr() + ", " ;
        it++;
    }

cout << "]";
cout << endl;
}

dPoint find_nearest_edge(dPoint p)
{
    int x=p.x;
    int y=p.y;

    int min_d = std::min({x, y, MAP_SIZE-x, MAP_SIZE-y });

    dPoint edge_pt;

        if(min_d == x)
            {
                edge_pt.x = 0;
                edge_pt.y = y;
            }
        else if(min_d == y)
            {   edge_pt.x = x;
                edge_pt.y = 0;
            }

        else if(min_d == MAP_SIZE-x)
        {
            edge_pt.x = MAP_SIZE-1;
            edge_pt.y = y;
        }

        else if(min_d == MAP_SIZE-y)
        {
            edge_pt.x = x;
            edge_pt.y = MAP_SIZE-1;

        }

        return edge_pt;
}

void travel_to(Robot& bot, dPoint p1, dPoint p2)
{

    Pair src = makepair(p1);
    Pair dest = makepair(p2);

    list<Pair> path_points = maint(grid, src, dest);
    cout<<endl;
    list<Pair>::iterator it = path_points.begin();
    for(int i=0; i<path_points.size(); i++)
        {
            dPoint dp = makepoint(*it);
            dp.print_pt();
            bot.set_xy(dp.x, dp.y);
            bot.printloc();
            it++;
        }





}

void goto_edge(Robot& bot)
{


        int x = bot.x;
        int y = bot.y;
        dPoint init(x, y);
        dPoint l;
        if(!(x == 0 || y == 0 || x == MAP_SIZE-1 || y == MAP_SIZE-1) )
       {

          l = find_nearest_edge(init);
          l.print_pt();
       }

       cout << "Travelling to  to edge" <<endl;

       travel_to(bot, init, l);
       //printf(plist.size());
       //print(plist);


    // getting size of the list
    int size = plist.size();

    //Point pp = get(plist, 0);
    //pp.print_pt();


}

void goto_corner(Robot& bot)
{
    cout<<"Travelling to corner"<<endl;
    //bot.printloc();
    int x = bot.getx();
    int y =bot.gety();
    int x2 =0 , y2 = 0;


    dPoint p1(x, y);
    //p1.print_pt();

    if(x == 0 || x == MAP_SIZE-1)
        {
            y2 = (y>MAP_SIZE/2)? MAP_SIZE-1:0 ;
        }
    else if(y== 0 || y == MAP_SIZE-1)
        {
            x2 = (y>MAP_SIZE/2)? MAP_SIZE-1:0 ;
        }

    dPoint p2(x2, y2);
    //p2.print_pt();
    travel_to(bot, p1, p2);
}
void sweep_empty(Robot& bot)
{

    goto_edge(bot);
    goto_corner(bot);

    bool upsweep = (bot.gety() == 0)? false:true ;
    bool rightsweep = (bot.getx() == 0)?true: false;
    //bot.printloc();
    list<dPoint> cleaned ;


    if(upsweep)
        {
            for(int r=MAP_SIZE-1 ; r>=0; r--)
                {
                    int inc = (rightsweep)? 1:-1;
                    int c = (rightsweep)? 0: MAP_SIZE-1;

                    for(int i=0;i<MAP_SIZE;i++)
                        {
                            dPoint p(c, r);
                            plist.push_back(p);
                            c+=inc;
                            p.print_pt();
                        }
                    rightsweep = !rightsweep;
                }
        }



}
void spiral_empty(Robot& bot)
{
    goto_edge(bot);
    //printlist(plist);
    cout << "At edge" <<endl;
    bot.printloc();


}

void find_obs_edges()
{

}

int map2d[64][64] = {};
void map_2d()
{
    int m = 64;
    int n = 64;

    for(int i=0; i<m; i++)
        {
            for(int j=0; j<n; j++)
                {

                    map2d[i][j] = ((int)image.at<uchar>(i,j)) > 200;
                    grid[i][j] = ((int)image.at<uchar>(i,j)) > 200;

                }


        }

}
queue<int> rq;
queue<int> cq;
bool visited[64][64];
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, +1, -1};


/*int explore_neighbours(int r, int c, int nodes_next)
{
    int rr, cc;

    for(int i=0; i<4; i++)
        {
            rr = r+dr[i];
            cc = c+dc[i];

            if(rr<0 || cc<0)
                { continue;
                }
            if (rr>=MAP_SIZE || cc>=MAP_SIZE)
                {continue;}

            if (visited[rr][cc])
                {continue;}

            if(map2d[cc][rr] < 200)
                {continue;}
            rq.push(rr);
            cq.push(cc);

            visited[rr][cc] = true;
            nodes_next++;

        }

        return nodes_next;
}

int bfs_solve(int sr, int sc, int er, int ec, bool reached_end, int nodes_left, int nodes_next, int move_count)
{
    rq.push(sr);
    cq.push(sc);

    visited[sr][sc] = true;
    while(rq.size()> 0)
        {
            int r = rq.front();
            int c = cq.front();

            if(r == er && c == ec)
                {
                    reached_end = true;
                    break;
                }
            nodes_next = explore_neighbours(r, c, nodes_next);
            nodes_left--;
            if(nodes_left == 0 )
                {
                    nodes_left = nodes_next;
                    nodes_next = 0;
                    move_count++;
                }

        }

        if (reached_end)
                {
                    return move_count;
                }
        return -1;

}
void bfs(dPoint p1, dPoint p2)
{
    queue<int> rq;
    queue<int> cq;

    int r = 64; int c = 64;

    int sr = p1.y; int sc = p1.x;
    int er = p2.y; int ec = p2.x;

    int move_count = 0;
    int nodes_left_in_layer = 1;
    int nodes_in_next_layer = 0;

    bool reached_end  = false;
    bool visited[r][c] = {{false}};



    bfs_solve(sr, sc, er, ec, reached_end, nodes_left_in_layer, nodes_in_next_layer, move_count );

}

*/

void travel_obs(Robot& bot)
{
}
void sweep_obs(Robot& bot)
{
    goto_edge(bot);
    goto_corner(bot);

}

void conv_to_grey()
{
    Mat image8u1;
    cvtColor(image, image8u1, COLOR_RGB2GRAY);

    image = image8u1.clone();

    //cout<<image<<endl;

    //bfs(64, 64, 34, 26, 0, 0);

    //string ty =  type2str( image.type() );
   // printf("Matrix: %s %dx%d \n", ty.c_str(), image.cols, image.rows );


}

int image_read()
{
    image = imread("/home/ayush/Desktop/Manas/Final/square_map.jpg");
    map_img = image.clone();

 if (image.empty())
 {
  cout << "Failed" << endl;
  cin.get();
  return -1;
 }

 String windowName = "Map";

 //namedWindow(windowName);
 //resizeWindow(windowName, 64*3, 64*3);
 //imshow(windowName, image); // Show our image inside the created window.
  //  imshow("cgrey", image);


 waitKey(0); // Wait for any keystroke in the window


 destroyWindow(windowName); //destroy the created window
 return 0;

}
int main()
{




    //r.p.print_pt();
    //sweep_empty(r);


    //sweep_obs(r);
    image_read();



    conv_to_grey();
    map_2d();


    Robot r(15, 12);
    dPoint pr = r.getpoint();
    dPoint p2(14, 4);
    travel_to(r, pr, p2);
    r.printloc();

    /*dPoint p1(12, 6);
    dPoint p2(32, 57);
    Pair src = makepair(p1);
    Pair dest = makepair(p2);

    list<Pair> path_points = maint(map2d, src, dest);
    cout<<endl;
    list<Pair>::iterator it = path_points.begin();
    for(int i=0; i<path_points.size(); i++)
        {
            dPoint dp = makepoint(*it);
            dp.print_pt();
            it++;
        }
    //dPoint p(6, 6); dPoint p1(0, 0);
    //bfs(p, p1);
    //sweep_obs(r);
*/

return 0;
}


