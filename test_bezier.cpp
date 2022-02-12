#include "src/bezier.h"
#include "matplotlibcpp/matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() {
    XY p0{0,0};
    XY p1{5,6};
    XY p2{10,8};
    XY p3{15,11};
    XY p4{20,14};
    XY p5{25, 18};



    XY q2{30,25};
    XY q3{35,27};
    XY q4{40,30};
    XY q5{45,34};

    XY h1{50,40};
        XY h2{55,42};
            XY h3{60,45};
                XY h4{65,50};

    XY aide1P{52.0711,39.6569};

    XY a2{75,54};
    XY a3{80,60};
    XY a4{85,64};
    XY a5{90,68};
                    

    std::vector<XY> points{p0, p1, p2, p3,p4, p5, q2, q3,q4,q5, h1,h2};

    std::vector<XY> points1{q5,aide1P,h1,h2,h3,h4, a2,a3,a4,a5};

    Bezier bezier(points);
    // bezier.showpoints();

    std::vector<Path> smoothpath = bezier.Smooth();

    bezier.showpoints();

    std::vector<double> path_X;
    std::vector<double> path_Y;
    std::vector<double> curvature;

    for (std::vector<Path>::iterator it = smoothpath.begin(); it!= smoothpath.end(); ++it) {
        double path_x_temp = it->xy.m_X;
        double path_y_temp = it->xy.m_Y;
        double curvature_temp = it->cur;
        
        path_X.push_back(path_x_temp);
        path_Y.push_back(path_y_temp);
        curvature.push_back(curvature_temp);
    }
    plt::figure(1);
    plt::plot(path_X,path_Y);
    plt::show();

    plt::figure(2);
    plt::plot(path_X, curvature);
    plt::show();

    return 0;
    
}