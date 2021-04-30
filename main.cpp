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

//string img_path = "/home/ayush/Desktop/Manas/Final/emptymap(1).jpg"; //OUTPUT 1
//string img_path = "/home/ayush/Desktop/Manas/Final/square_map.jpg"; //OUTPUT 2
//string img_path = "/home/ayush/Desktop/Manas/Final/map_w_circle.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/map_w_circle2.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/map_w_circle3.jpg"; // good output
//string img_path = "/home/ayush/Desktop/Manas/Final/mult_circles.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/face_map(1).jpg";


//MORE TEST MAPS

//string img_path = "/home/ayush/Desktop/Manas/Final/test_map_1.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/test_map_2.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/test_map_4.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/test_map_5.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/test_map_6.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/test_map_7.jpg";

//string img_path = "/home/ayush/Desktop/Manas/Final/test_map500_1.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/test_map500_2.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/test_map500_3.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/test_map500_4.jpg";
string img_path = "/home/ayush/Desktop/Manas/Final/test_map500_5.jpg";
//string img_path = "/home/ayush/Desktop/Manas/Final/test_map500_6.jpg";

int radius = 4;

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
dPoint(int xa, int ya)
    {
        x = xa;
        y = ya;

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

     dPoint getNextPoint(int dir)
    {
        int xa; int ya;
        switch(dir)
        {
        case 0:
            xa = x;
            ya = y-radius;
            break;
        case 1:
            xa = x+radius;
            ya = y;
            break;
        case 2:
            xa = x;
            ya = y+radius;
            break;
        case 3:
           xa = x-radius;
            ya = y;
            break;

        }

        dPoint dp(xa, ya);
        return dp;






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
list<dPoint> cleaned ;
list<dPoint> overlap ;

dPoint get(list<dPoint> _list, int _i){
    list<dPoint>::iterator it = _list.begin();
    for(int i=0; i<_list.size(); i++){

        ++it;
    }
    return *it;
}
bool equalsPoint(dPoint& it, dPoint& dp)
{
    if((it).x == dp.x && it.y == dp.y)
        {
            return true;
        }
    return false;

}

double getPointDist(dPoint p1, dPoint p2)
{
    return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}
void push_unq(list<dPoint> _list, dPoint dp)
{
     //list<dPoint>::iterator it = _list.begin();
    // cout<<"TRYING TO PUSH UNQ"<<endl;
    for(int i=0; i<_list.size(); i++)
        {dPoint it = get(_list, i);

        if(equalsPoint(it, dp))
            {
                // cout<<"RETURNED"<<endl;
                return;
            }


    }
//cout<<"POINT ADDED"<<endl;
    _list.push_back(dp);

}


int MAP_SIZE = 500;

int map2d[500][500] = {};
int grid[500][500]= {};
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
int overlap_count = 0;
int non_ov = 0;

int clnd = 0;

void update_grid(dPoint pt)
{
    int x = pt.x; int y = pt.y;
    int x1 = (x-radius > 0)? x - radius : 0;
    int y1 = (y-radius > 0)? y - radius : 0;

    bool all_over = true;
    for(;x1 <= x+radius && x1< MAP_SIZE; x1++)
        {
            for(y1 = y -radius; y1<=y+radius && y1<MAP_SIZE; y1++)
            {
                if (getPointDist(dPoint(x1, y1), pt) <= radius)
                    {
                        if(grid[y1][x1] != 2)
                            all_over = false;
                        grid[y1][x1] = 2;

                    }
            }
        }

        if(all_over)
        {circle(map_img, Point(x, y), radius, Scalar(255, 0, 0), -1, 8);}

}
void clean_pt(dPoint pt)
{

    int x = pt.x;
    int y = pt.y;
    radius = 4;


     cleaned.push_back(pt);

     unsigned char * p = map_img.ptr(y, x); // Y first, X after

     circle(map_img, Point(x, y), radius, Scalar(0, 0, 255), -1, 8);
     if(grid[y][x] == 2)
     {
                                    p[0] = 255;   // B
                                    p[1] = 0;   // G
                                    p[2] = 0; // R

                                    overlap_count++;
     }

     else {
            grid[y][x] = 2;
                                    p[0] = 0;   // B
                                    p[1] = 0;   // G
                                    p[2] = 255; // R

                                    non_ov++;


}

update_grid(pt);
}

void travel_to(Robot& bot, dPoint p1, dPoint p2)
{

    Pair src = makepair(p1);
    Pair dest = makepair(p2);

    list<Pair> path_points = maint(map2d, src, dest);
    cout<<endl;
    list<Pair>::iterator it = path_points.begin();
    for(int i=0; i<path_points.size(); i++)
        {
            dPoint dp = makepoint(*it);
            dp.print_pt();
            bot.set_xy(dp.x, dp.y);
            if(grid[dp.y][dp.x] == 2)
                {
                    //overlap_count++;
                }
            else{
                //non_ov++;
            }

            /*grid[dp.y][dp.x] = 2;
             unsigned char * p = map_img.ptr(dp.y, dp.x); // Y first, X after
                p[1] = 0;   // G
                p[2] = 255; // R
                p[0] = 0;   // B
                */
            clean_pt(dp);
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

void map_2d()
{
    int m = MAP_SIZE;
    int n = MAP_SIZE;

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
bool visited[500][500];
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, +1, -1};

int steps = 0;



void travel_obs(Robot& bot)
{
}
int checked[500][500] = {{}};
/*void scope_free(int i, int j)
{
    if(checked[i][j] == 2)
        {
            return;
        }
    bool free = true;
                            int c_free =0;

                            for(int inc = 0; inc<4 && free; inc++)
                            {for(int m = i; (m>=0 && m<MAP_SIZE && free); m+=dr[inc])
                                {
                                    for(int k =0;k<4  && free; k++)
                                        {
                                        for(int n=j; n>=0 && n<MAP_SIZE  && free;n+=dr[k] )
                                            {
                                                if(grid[m][n] == 2 ||grid[m][n] == 0 )
                                                    {
                                                        free =false;
                                                        break;

                                                    }
                                                    checked[i][j] = 2;
                                                    c_free++;
                                            }
                                    }
                                }
                            }


                        cout<<"Free at " + to_string(i) + " (row) , col = "+to_string(j)<<endl;
                        cout<<c_free<<endl;


}
*/
int notcleaned()
{
    int clnd = 0;
    int count_ = 0;
    for(int i=0; i<MAP_SIZE; i++)
        {
            for(int j=0; j<MAP_SIZE; j++)
                {
                    if(grid[i][j] == 1 )
                        {
                            count_++;
                           // scope_free( i,  j);
                        }
                    if(grid[i][j] == 2 )
                        {
                            clnd++;
                        }
                }
        }
     //   cout << "NOT CLEANED \t\t" + to_string(count_) << endl;
      //  cout << "CLEANED \t\t" + to_string(clnd) << endl;
        return count_;
}

int find_overlap_path(dPoint p1, dPoint p2)
{

    Pair src = makepair(p1);
    Pair dest = makepair(p2);
    int overlap = 0;
    list<Pair> path_points = maint(map2d, src, dest);
    cout<<endl;
    list<Pair>::iterator it = path_points.begin();
    for(int i=0; i<path_points.size(); i++)
        {
            dPoint dp = makepoint(*it);

            if(grid[dp.y][dp.x] == 2)
                {
                    overlap++;
                }

            it++;
        }
        return overlap;
}

dPoint find_notcl(Robot& bot)
{
    list<dPoint> dl;
    float min_dis = 500*500;
    int min_ov = 500*500;
    dPoint mindisp;
    float dis=0;

    for(int i=0; i<MAP_SIZE; i++)
        {
            for(int j=0; j<MAP_SIZE; j++)
                {
                    if(grid[i][j] == 1 && i!=0 && j!=0 && i!=MAP_SIZE-1 && j!=MAP_SIZE-1 )
                        {
                            dPoint dp(j, i);
                           // cout<<"NOT CLEANED:: \t\t"<<endl;
                           // dp.print_pt();
                            dl.push_back(dp);

                           // int ovp = find_overlap_path(bot.getpoint(), dp);
                            /*dis = getPointDist( bot.getpoint(), dp);

                            if(dis < min_dis)
                                {min_dis = dis;
                                mindisp = dp;

                            }


                            if(ovp < min_ov)
                                {min_ov = ovp;
                                mindisp = dp;

                            }
                        */


                        }
                }


        }

  //      return mindisp;

// DE-COMMENT IF WANT TO TEST MAX_ROW APPROACH
        dPoint maxp;
        int indx;
        double maxd = 0;
        int max_im;
        list<dPoint>::iterator it = dl.begin();
        for(int i=0; i<dl.size(); i++)
            {
                int m_count = 0;
                if(i ==0)
                    max_im = m_count;


                for(int m=(*it).x; m<MAP_SIZE && grid[(*it).y][m] == 1;m++)
                    {
                        if(grid[(*it).y][m] == 1 )
                            m_count++;

                    }
                for(int n = (*it).y; n<MAP_SIZE && grid[n][(*it).x] == 1; n++)
                    {
                        if(grid[n][(*it).x] == 1 )
                        m_count++;
                    }
                if(max_im < m_count)
                    {
                        cout << "FOUND THIS M_COUNT"+to_string(m_count)<<endl;
                        max_im = m_count;
                        indx = i;
                    }
                it++;
            }

        list<dPoint>::iterator it2 = dl.begin();
        for(int i=0; i<indx; i++)
            {it2++;
            }
        //cout<<"MOST NOTCLEANED POTINT IS THIS:::"<<endl;
        //cout<<"THIS IS IT" + to_string(grid[(*it2).y][(*it2).x])<<endl;
        grid[(*it2).y][(*it2).x] = 2;
        (*it2).print_pt();
        cout<<max_im<<endl;
        return (*it2);

        }


bool isBlocked(dPoint p)
{
    if(map2d[p.y][p.x] == 0)
        {
            return true;
        }
    return false;
}

bool check_uncl_row(dPoint pt, bool right_or_left)
{
    int x = pt.x;
    int y = pt.y;
    int inc = (right_or_left)? 1:-1;
    int count_ones = 0;
    for(int c = x; c<MAP_SIZE && c>=0 && !isBlocked(pt); c+=inc)
        {
            if(grid[y][c] == 1)
                {
                return true;
            }
            pt.x = c;
        }

        return false;
}

void sweep_obs(Robot& bot)
{
    int num_cycles = 0;
    //goto_edge(bot);
    //goto_corner(bot);



    while(notcleaned() > 0) //&& cleaned.size() < 64*64*2)
        {   cout<<"NOT CLND: "+ to_string(notcleaned())<<endl;


            dPoint current = bot.getpoint();

             if(num_cycles == 1)
                    {
                    cout<<"First new cycle"<<endl;
                    current.print_pt();
                    //cin.get();
                    }
            int x = current.x;
            int y = current.y;
            bool right_sweep = true;
            int right_or_left = 1;
            bool is_clean = false;
            bool bb = isBlocked(bot.getNextPoint(2));

            int inc_y = 8;

            cout << "BLOCKD: " +to_string(bb) << endl;
            //while(x<MAP_SIZE && y<MAP_SIZE && !isBlocked(bot.getpoint()))
            //while(x<MAP_SIZE && y<MAP_SIZE && !isBlocked(bot.getNextPoint(2)))
             do   {

                            current = bot.getpoint();
                            x = current.x;
                            y = current.y;




                            clean_pt(current);

                            right_or_left = (right_sweep)? 1:3;
                            int inc = (right_sweep)? 1:-1 ;
                            bot.setdir(right_or_left);

                            /*
                            bot.printloc();
                            cout << to_string(isBlocked(bot.getNextPoint(right_or_left)))<<endl;
                            string ss = (right_sweep)?"right":"left";
                            cout<<ss<<endl;
                            cout << to_string(x) +"is x; y is" + to_string(y)<<endl;
                            */
                            //while(!isBlocked(bot.getNextPoint(right_or_left)))
                            if(check_uncl_row(current, right_sweep))
                           {
                           do
                               {
                                    //if(notcleaned()<=5)
                                        //{break;}

                                    if(grid[y][x] != 2)
                                        {
                                            //non_ov++;
                                        }
                                    else
                                        {//overlap_count++;
                                        }


                                 if(right_sweep && x<MAP_SIZE-1 && y<MAP_SIZE-1)
                                   { current = bot.getpoint();
                                       // push_unq(cleaned, current );
                                        clean_pt(current);


                                        //grid[y][x] = 2; // 2 for cleaned, 1 for uncleaned, 0 for obs

                                        bot.move_bot(1);
                                        //cout<<"Sweep loc: ";
                                        //bot.printloc();
                                }
                                else if(!right_sweep && x>0 && y>0   )
                                    {
                                        current = bot.getpoint();
                                       // push_unq(cleaned, current );
                                        clean_pt(current);


                                        //grid[y][x] = 2; // 2 for cleaned, 1 for uncleaned, 0 for obs

                                        bot.move_bot(1);
                                    }
                                    else
                                        {
                                            break;
                                        }

                                    //map_img.at<uchar>(i,j)) = 200;)


                                    x+=inc;
                                    if(cleaned.size()%2000 == 0 )
                                        cout<<"CLEANED: "+ to_string(cleaned.size())<<endl;

                                    unsigned char * p = map_img.ptr(y, x); // Y first, X after

                                    String windowName = "Map";
                                /*


                                    p[0] = 0;   // B
                                    p[1] = 0;   // G
                                    p[2] = 255; // R

                            */
                                    imshow("MAP", map_img);


                                     if (image.empty())
                                         {
                                          cout << "Failed" << endl;
                                          //cin.get();
                                          return ;
                                         }

                         windowName = "Map";

                         namedWindow(windowName);
                         imshow(windowName, map_img); // Show our image inside the created window.



                         waitKey(1); // Wait for any keystroke in the window


                         destroyWindow(windowName); //destroy the created window
                                                        //printlist(cleaned);


                                }while(!isBlocked(bot.getNextPoint(right_or_left)));

                           } // end of if check_uncl_row

                                current = bot.getpoint();
                                //push_unq(cleaned, current );

                                clean_pt(current);

                                bot.setdir(2);
                                bot.move_bot(inc_y);

                                y+=inc_y;
                                /*cout << "MOVED DOWN TO y : : : "+to_string(y)<< endl;
                                cout << cleaned.size()<< endl;
                                */
                                bot.printloc();
                                right_sweep = !right_sweep;

                        }while(x<MAP_SIZE && y<MAP_SIZE && !isBlocked(bot.getNextPoint(2)));


               dPoint not_cl = find_notcl(bot);
            cout << "CURRENT POINT :: : : : "<< endl;
                current.print_pt();
                cout<<map2d[current.y][current.x]<<endl;
                map2d[current.y][current.x] = 1;
            cout << "NEXT POINT :: : : : "<< endl;
            cout<<map2d[not_cl.y][not_cl.x]<<endl;
            not_cl.print_pt();
            num_cycles++;

            if(notcleaned()<=5)
                   {break;}

            //cin.get();
               // return;

           travel_to(bot, current, not_cl);

        }

        steps = overlap_count + non_ov;
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
    image = imread(img_path);
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
  //imshow("cgrey", image);


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

//    if(img_path.compare())
   Robot r(15, 12);
     //Robot r(0, 0);
    dPoint pr = r.getpoint();

    dPoint p2(0,0);
    travel_to(r, pr, p2);
    r.printloc();

    sweep_obs(r);

    for(int i=0; i<MAP_SIZE;i++)
        {
            for(int j=0; j<MAP_SIZE;j++)

                {
                    cout<<map2d[i][j];
                    cout<<" ";
                }
                cout<<endl;
        }


        cout<<"OVERLAP::";
        cout<<overlap_count<<endl;

        cout<<"NON Overlap::";
        cout<<non_ov<<endl;

        cout<<"\nOverlap % = "+to_string(((float)(steps - non_ov ))/non_ov)+" %\n";

        cout<<"CLEANING DONE"<<endl;



        String windowName = "Map";

 //namedWindow(windowName);
 //imshow(windowName, map_img); // Show our image inside the created window.

  waitKey(0);
  destroyAllWindows();




return 0;
}


