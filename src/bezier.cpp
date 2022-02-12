#include "bezier.h"

/**
 * 输入： 粗糙的路径点
 * 输出： 无
 * 作用： 构造函数
**/
Bezier::Bezier(const std::vector<XY>& rough_path) {    
    this->way_points = rough_path;
    Judge_Order_and_SmoothType();
    divide_way_points();
};

/**
 * 输入： 无
 * 输出： 无
 * 作用： 判断主体贝塞尔曲线阶数，并判断最后两条贝塞尔曲线拼接类型
**/
void Bezier::Judge_Order_and_SmoothType(){
    // 粗糙路径点的个数
    size_t number_of_points = this->way_points.size();
    // 如果路径点个数在3~6之间 只使用三阶贝塞尔曲线进行平滑
    if (number_of_points < 6 && number_of_points > 3) {
        this->main_order = 3;
        this->Smooth_merge_type = NO_MERGE;
        this->number_of_line = 1;
    }
    // 如果路径点个数在5~8之间 只使用五阶贝塞尔曲线进行平滑
    else if (number_of_points >5 && number_of_points < 8) {
        this->main_order = 5;
        this->Smooth_merge_type = NO_MERGE;
        this->number_of_line = 1;
    }
    // 如果路径点在 7~12 之间 根据点的不同选用不同的拼接方式  拼接曲线 ： 五阶-4个控制点 三阶-2个控制点
    else if (number_of_points > 7 && number_of_points < 12) {
        this->main_order = 5;
        this->number_of_line = 2;
        int remainder = number_of_points % 6;
        switch (remainder)
        {
        case 2:
            this->Smooth_merge_type = FIVE_THREE;
            break;
        case 3:
            this->Smooth_merge_type = FIVE_THREE;
            break;
        case 4:
            this->Smooth_merge_type = FIVE_FIVE;
            break;
        case 5:
            this->Smooth_merge_type = FIVE_FIVE;
        default:
            break;
        }  
    }
    else if (number_of_points > 11) {
        this->main_order = 5;
        int remainder = (number_of_points - 6) % 4; //余数
        int quotient = (number_of_points - 6) / 4;  // 商
        switch (remainder)
        {
        case 0:
            this->Smooth_merge_type = FIVE_FIVE;
            this->number_of_line = quotient + 1;
            break;
        case 1:
            this->Smooth_merge_type = FIVE_FIVE;
            this->number_of_line = quotient + 1;
            break;
        case 2:
            this->Smooth_merge_type = FIVE_THREE;
            this->number_of_line = quotient + 2;
            break;
        case 3:
            this->Smooth_merge_type = FIVE_THREE;
            this->number_of_line = quotient + 2;
            break;
        default:
            break;
        }
    }
};

/**
 * 输入： 上一段贝塞尔曲线的后三个控制点 p5p4p3, 以及下一段贝塞尔曲线的第三个点 Q2 , Q0 = p5, 辅助控制点Q1计算
 * 输出： 辅助控制点Q1 XY坐标
 * 作用： 计算辅助控制点 使曲线连接处满足 G0、G1、G2连续
**/
void Bezier::divide_way_points() {
    if (this->number_of_line == 1) {
        switch (this->main_order)
        {
        case 3:{
            std::vector<XY> cubic_control_points;
            cubic_control_points.reserve(4);
            int i = 0;
            for (std::vector<XY>::iterator it = this->way_points.begin();
                it!= this->way_points.end();
                ++it) {
                cubic_control_points.push_back(*it);
                ++i;
                if (i>4) {
                    break;
                }
            }
            this->control_Points.push_back(cubic_control_points);
            break;
        }
        case 5:{
            std::vector<XY> quintic_control_points;
            quintic_control_points.reserve(6);
            int i =0;
            for (std::vector<XY>::iterator it = this->way_points.begin();
                it!= this->way_points.end();
                ++it) {
                quintic_control_points.push_back(*it);
                ++i;
                if (i>5) {
                    break;
                }
            }
            this->control_Points.push_back(quintic_control_points);
            break;
        }
        default:
            break;
        }
    }
    else if (this->number_of_line == 2) {
        switch (this->Smooth_merge_type)
        {
        case FIVE_THREE:{
            std::vector<XY> cubic_control_points;
            std::vector<XY> quintc_cotrol_points;
            cubic_control_points.reserve(4);
            quintc_cotrol_points.reserve(6);
            int i =0;
            for (std::vector<XY>::iterator it=this->way_points.begin();
                    it!= this->way_points.end();
                    ++it) {
                if(i<6) {
                    quintc_cotrol_points.push_back(*it);
                }
                else if (i>5 && i<8) {
                    cubic_control_points.push_back(*it);
                }
                else {
                    break;
                }
                ++i;
            }
            this->control_Points.push_back(quintc_cotrol_points);
            this->control_Points.push_back(cubic_control_points);
            break;
        }
        case FIVE_FIVE:{
            std::vector<XY> quintc_control_points0;
            std::vector<XY> quintc_control_points1;
            quintc_control_points0.reserve(6);
            quintc_control_points1.reserve(4);
            int i =0;
            for (std::vector<XY>::iterator it=this->way_points.begin();
                    it!= this->way_points.end();
                    ++it) {
                if(i<6) {
                    quintc_control_points0.push_back(*it);
                }
                else if (i>5 && i<10) {
                    quintc_control_points1.push_back(*it);
                }
                else {
                    break;
                }
                ++i;
            }
            this->control_Points.push_back(quintc_control_points0);
            this->control_Points.push_back(quintc_control_points1);
            break;
        }
        default:
            break;
        }
    }
    else if (this->number_of_line > 2) {
        switch (this->Smooth_merge_type)
        {
        case FIVE_THREE:{
            std::vector<XY>::const_iterator head = this->way_points.begin();
            std::vector<XY>::const_iterator tail = this->way_points.begin() + 6;
            for (size_t i = 0; i<this->number_of_line; ++i) {
        
                if (i == 0) {
                    std::vector<XY> quintic_control_points0(head,tail);
                    this->control_Points.push_back(quintic_control_points0);
                    // std::cout<< " ---1--- " << '\n';
                }

                else if (i>0 && i < this->number_of_line-1) {
                    head = tail;
                    tail = head + 4;
                    std::vector<XY> quintic_control_poinst1(head,tail);
                    this->control_Points.push_back(quintic_control_poinst1);
                    // std::cout<< " ---2--- " << '\n';

                }
                else if (i == this->number_of_line-1) {
                    head = tail;
                    tail = head + 2;
                    std::vector<XY> cubic_control_points(head,tail);
                    this->control_Points.push_back(cubic_control_points);
                    // std::cout<< " ---3--- " << '\n';

                }
            }
            break;
        }
        case FIVE_FIVE:{
            std::vector<XY>::const_iterator head = this->way_points.begin();
            std::vector<XY>::const_iterator tail = this->way_points.begin() + 6;
            for (size_t i = 0; i<this->number_of_line; ++i) {

                if (i == 0) {
                    std::vector<XY> quintic_control_points0(head,tail);
                    this->control_Points.push_back(quintic_control_points0);
                }

                else if (i>0 && i < this->number_of_line) {
                    head = tail;
                    tail = head + 4;
                    std::vector<XY> quintic_control_poinst1(head,tail);
                    this->control_Points.push_back(quintic_control_poinst1);
                }
            }
            break;
        }

        default:
            break;
        }
    }

    // std::cout<<this->control_Points.size() <<'\n';
};


/**
 * 输入： 上一段贝塞尔曲线的后三个控制点 p5p4p3, 以及下一段贝塞尔曲线的第三个点 Q2 , Q0 = p5, 辅助控制点Q1计算
 * 输出： 辅助控制点Q1 XY坐标
 * 作用： 计算辅助控制点 使曲线连接处满足 G0、G1、G2连续
**/
XY Bezier::compute_aide_point(XY P3, XY P4, XY P5, XY Q2, MergeType T){
    // G1 lianxu   p5-p4 = n(q1-q0)  0<n<1;
    // double nita =0;

    // solve c1
    double g1 = P5.m_X - P4.m_X;
    double g2 = P4.m_X - P3.m_X;
    double h1 = P5.m_Y - P4.m_Y;
    double h2 = P4.m_Y - P3.m_Y;
    double m = 4*abs(g1*(h1-h2) - h1*(g1-g2));
    double ss = 5*std::pow(g1*g1+h1*h1,1.5);
    double c1 = m/ss;  // curvature of first curve
    
    std::cout<<"c1  " <<c1<<'\n';
    // double minus_min = std::numeric_limits<double>::max();

    XY aide_point_Q1;

    switch (T)
    {
    case FIVE_FIVE:{
        // bianlifa qiu jinsijie
        // for (double n=0.000001 ; n<1;n+=0.0001) {
        //     XY Q1 = (P5*(1+n) - P4) / n;
        //     Q1.m_X = (P5.m_X*(1+n) - P4.m_X) / n;
        //     Q1.m_Y = (P5.m_Y*(1+n) - P4.m_Y) / n;

        //     double m_ = abs(5*(Q1.m_X - P5.m_X)*(Q2.m_Y - 2*Q1.m_Y + P5.m_Y)*20 - 20*(Q2.m_X - 2*Q1.m_X + P5.m_X)*5*(Q1.m_Y - P5.m_Y));
        //     double ss_ = std::pow((25*std::pow(Q1.m_X - P5.m_X,2) + 25*std::pow(Q1.m_Y - P5.m_Y,2)),1.5);
        //     double c2 = m_/ss_;
        //     double minus = abs(c1-c2);

        //     if (minus < minus_min) {
        //         minus_min = minus;
        //         aide_point_Q1 = Q1;
        //         // std::cout<< c2<<'\n';
        //     }
    // }
        double g = P5.m_X - P4.m_X;
        double h = P5.m_Y - P4.m_Y;
        double r = std::pow((g*g + h*h),1.5);
        double n = std::sqrt(abs(g*(Q2.m_Y - P5.m_Y) - h*(Q2.m_X - P5.m_X)) / (1.25*c1*r)) ;
            
        if (c1 == 0) {
            aide_point_Q1.m_X = P5.m_X;
            aide_point_Q1.m_Y = (h*aide_point_Q1.m_X - h*Q2.m_X + g*Q2.m_Y) / g ;
        }
        else {
            
            // std::cout<< "n=" << n<< '\n';
            aide_point_Q1.m_X = n*g + P5.m_X;
            aide_point_Q1.m_Y = n*h + P5.m_Y;
        }
        break;
    }

    case FIVE_THREE:{

        // for (double n=0.0000001 ; n<2.667; n+=0.0001) {
            
        //     XY Q1; 
        //     Q1.m_X = (P5.m_X*(5+3*n) - P4.m_X*5) / (3*n);
        //     Q1.m_Y = (P5.m_Y*(5+3*n) - P4.m_Y*5) / (3*n);

        //     double m_ = abs(3*(Q1.m_X - P5.m_X)*(Q2.m_Y - 2*Q1.m_Y + P5.m_Y)*6 - 6*(Q2.m_X - 2*Q1.m_X + P5.m_X)*3*(Q1.m_Y - P5.m_Y));
        //     double ss_ = std::pow((9*std::pow(Q1.m_X - P5.m_X,2) + 9*std::pow(Q1.m_Y - P5.m_Y,2)),1.5);
        //     double c2 = m_/ss_;
        //     double minus = abs(c1-c2);

        //     if (minus < minus_min) {
        //         minus_min = minus;
        //         aide_point_Q1 = Q1;
        //     }
        // }
        double g = P5.m_X - P4.m_X;
        double h = P5.m_Y - P4.m_Y;
        double r = std::pow((g*g + h*h),1.5);
        double n = std::sqrt(abs(g*(Q2.m_Y - P5.m_Y) - h*(Q2.m_X - P5.m_X)) / (1.5*c1*r)) ;
            
        if (c1 == 0) {
            aide_point_Q1.m_X = P5.m_X;
            aide_point_Q1.m_Y = (h*aide_point_Q1.m_X - h*Q2.m_X + g*Q2.m_Y) / g ;
        }
        else {
            
            // std::cout<< "n=" << n<< '\n';
            aide_point_Q1.m_X = n*g + P5.m_X;
            aide_point_Q1.m_Y = n*h + P5.m_Y;
        }

        break;
    }

    default:
        break;
    }
    return aide_point_Q1;
}

/**
 * 输入： 控制点
 * 输出： 坐标点
 * 作用： 计算五阶贝塞尔曲线坐标点
**/
std::vector<Path> Bezier::compute_Quintic(XY& point0, XY& point1, XY& point2,
                                XY& point3, XY& point4, XY& point5) {
    std::vector<Path> beziercurve;
    for (double t=0; t<=1; t+=0.001) {
        double x = point0.m_X*std::pow((1-t),5) + 5*point1.m_X*t*std::pow((1-t),4) +
                    10*point2.m_X*std::pow(t,2)*std::pow((1-t),3) + 10*point3.m_X*std::pow(t,3)*std::pow((1-t),2) +
                    5*point4.m_X*std::pow(t,4)*(1-t) + point5.m_X*std::pow(t,5);

        double y = point0.m_Y*std::pow((1-t),5) + 5*point1.m_Y*t*std::pow((1-t),4) +
                    10*point2.m_Y*std::pow(t,2)*std::pow((1-t),3) + 10*point3.m_Y*std::pow(t,3)*std::pow((1-t),2) +
                    5*point4.m_Y*std::pow(t,4)*(1-t) + point5.m_Y*std::pow(t,5);

        XY xy_temp;
        xy_temp.m_X = x;
        xy_temp.m_Y = y;

        double dot_x = 5*(point1.m_X - point0.m_X)*std::pow((1-t),4) + 20*(point2.m_X-point1.m_X)*t*std::pow((1-t),3) + 
                        30*(point3.m_X - point2.m_X)*std::pow(t,2)*std::pow((1-t),2) + 20*(point4.m_X-point3.m_X)*std::pow(t,3)*(1-t) +
                        5*(point5.m_X-point4.m_X)*std::pow(t,4);

        double dot_y = 5*(point1.m_Y - point0.m_Y)*std::pow((1-t),4) + 20*(point2.m_Y-point1.m_Y)*t*std::pow((1-t),3) + 
                        30*(point3.m_Y - point2.m_Y)*std::pow(t,2)*std::pow((1-t),2) + 20*(point4.m_Y-point3.m_Y)*std::pow(t,3)*(1-t) +
                        5*(point5.m_Y-point4.m_Y)*std::pow(t,4);

        double dot_dot_x = 20*(point2.m_X - 2*point1.m_X + point0.m_X)*std::pow((1-t),3) + 
                            60*(point3.m_X - 2*point2.m_X + point1.m_X)*t*std::pow((1-t),2) +
                            60*(point4.m_X - 2*point3.m_X + point2.m_X)*std::pow(t,2)*(1-t) +
                            20*(point5.m_X - 2*point4.m_X + point3.m_X)*std::pow(t,3);

        double dot_dot_y = 20*(point2.m_Y - 2*point1.m_Y + point0.m_Y)*std::pow((1-t),3) + 
                            60*(point3.m_Y - 2*point2.m_Y + point1.m_Y)*t*std::pow((1-t),2) +
                            60*(point4.m_Y - 2*point3.m_Y + point2.m_Y)*std::pow(t,2)*(1-t) +
                            20*(point5.m_Y - 2*point4.m_Y + point3.m_Y)*std::pow(t,3);

        double curvature = (dot_x*dot_dot_y - dot_dot_x*dot_y) / 
                            std::pow((std::pow(dot_x,2)+std::pow(dot_y,2)),1.5);

        Path path_temp;
        path_temp.xy = xy_temp;
        path_temp.cur = curvature;
        // todo theta;

        beziercurve.push_back(path_temp);
    }
    return beziercurve;
}

/**
 * 输入： 无
 * 输出： 坐标点
 * 作用： 计算三阶贝塞尔曲线坐标点
**/
std::vector<Path>  Bezier::compute_Cubic(XY& point0, XY& point1, XY& point2, XY& point3) {
    std::vector<Path> bezier_curve;
    for (double t = 0; t<1; t+=0.001) {
        double x = point0.m_X*std::pow(1-t,3) + 3*point1.m_X*t*std::pow(1-t,2) + 3*point2.m_X*t*t*(1-t) +point3.m_X*std::pow(t,3);
        double y = point0.m_Y*std::pow(1-t,3) + 3*point1.m_Y*t*std::pow(1-t,2) + 3*point2.m_Y*t*t*(1-t) +point3.m_Y*std::pow(t,3);

        XY xy_temp{x,y};

        double dot_x = 3*(point1.m_X - point0.m_X)*(1-t)*(1-t) + 6*(point2.m_X - point1.m_X)*t*(1-t) + 3*(point3.m_X - point2.m_X)*t*t;
        double dot_y = 3*(point1.m_Y - point0.m_Y)*(1-t)*(1-t) + 6*(point2.m_Y - point1.m_Y)*t*(1-t) + 3*(point3.m_Y - point2.m_Y)*t*t;
        
        double dot_dot_x = 6*(point2.m_X - 2*point1.m_X + point0.m_X)*(1-t) + 6*(point3.m_X - 2*point2.m_X + point1.m_X)*t;
        double dot_dot_y = 6*(point2.m_Y - 2*point1.m_Y+ point0.m_Y)*(1-t) + 6*(point3.m_Y - 2*point2.m_Y + point1.m_Y)*t;

        double curvature = (dot_x*dot_dot_y - dot_dot_x*dot_y) / 
                            std::pow((std::pow(dot_x,2)+std::pow(dot_y,2)),1.5);

        Path path_temp;
        path_temp.xy = xy_temp;
        path_temp.cur = curvature;

        bezier_curve.push_back(path_temp);
    }
    return bezier_curve;
}

/**
 * 输入： 无
 * 输出： 期望转角
 * 作用： 计算路径期望转角
**/
double Bezier::compute_theta() {
    double a = 10;
    return a;
}

/**
 * 输入： 无
 * 输出： 平滑后的路径
 * 作用： 平滑路径
**/
std::vector<Path> Bezier::Smooth() {

    std::vector<Path> smooth_path;

    if (this->number_of_line == 1) {
        switch (this->main_order)
        {
        case 3:
            smooth_path = compute_Cubic(this->control_Points[0][0], this->control_Points[0][1],
                                        this->control_Points[0][2], this->control_Points[0][3]);
            break;
        case 5:
            smooth_path = compute_Quintic(this->control_Points[0][0], this->control_Points[0][1],
                                            this->control_Points[0][2], this->control_Points[0][3],
                                            this->control_Points[0][4], this->control_Points[0][5]);
        default:
            break;
        }
    }

    else if (this->number_of_line == 2) {
        switch (this->Smooth_merge_type)
        {
        case FIVE_THREE:{
            XY aide_points = compute_aide_point(this->control_Points[0][3], this->control_Points[0][4],
                                                this->control_Points[0][5], this->control_Points[1][0], FIVE_THREE);
            this->control_Points[1].insert(this->control_Points[1].begin(), aide_points);
            this->control_Points[1].insert(this->control_Points[1].begin(), this->control_Points[0][5]);
            
            smooth_path  = compute_Quintic(this->control_Points[0][0], this->control_Points[0][1],
                                                            this->control_Points[0][2], this->control_Points[0][3],
                                                            this->control_Points[0][4], this->control_Points[0][5]);
            std::vector<Path> smooth_path1  = compute_Cubic(this->control_Points[1][0], this->control_Points[1][1],
                                                                            this->control_Points[1][2], this->control_Points[1][3]);
            
            smooth_path.reserve(smooth_path.size() + smooth_path1.size());
            smooth_path.insert(smooth_path.end(), smooth_path1.begin(), smooth_path1.end());
            break;
        }
        case FIVE_FIVE:{
            XY aide_points = compute_aide_point(this->control_Points[0][3], this->control_Points[0][4],
                                                this->control_Points[0][5], this->control_Points[1][0], FIVE_FIVE);
            // jiang jisuan chu de fuzhukongzhidian he qidian charudao shuzu de toubuweizhi
            this->control_Points[1].insert(this->control_Points[1].begin(), aide_points);
            this->control_Points[1].insert(this->control_Points[1].begin(), this->control_Points[0][5]);

            smooth_path  = compute_Quintic(this->control_Points[0][0], this->control_Points[0][1],
                                                            this->control_Points[0][2], this->control_Points[0][3],
                                                            this->control_Points[0][4], this->control_Points[0][5]);

            std::vector<Path> smooth_path1  = compute_Quintic(this->control_Points[1][0], this->control_Points[1][1],
                                                            this->control_Points[1][2], this->control_Points[1][3],
                                                            this->control_Points[1][4], this->control_Points[1][5]);
            // jiangliangduan lujing hebin wei yizhengge lujin
            smooth_path.reserve(smooth_path.size() + smooth_path1.size());
            smooth_path.insert(smooth_path.end(), smooth_path1.begin(), smooth_path1.end());
            break;
        }
        default:
            break;
        }
    }

    // cunyi ???? todo
    else if (this->number_of_line > 2) {

        smooth_path = compute_Quintic(this->control_Points[0][0], this->control_Points[0][1],
                                        this->control_Points[0][2], this->control_Points[0][3],
                                        this->control_Points[0][4], this->control_Points[0][5]);

        switch (this->Smooth_merge_type)
        {
        case FIVE_THREE:{
            
            for (int i=0; i < this->number_of_line-1;++i) {
                // zhongjian de lujing you wujie pinjie ercheng  sikao!!!!!
                if (i< this->number_of_line - 2) {
                    XY aide_point = compute_aide_point(this->control_Points[i][3], this->control_Points[i][4], 
                                                        this->control_Points[i][5], this->control_Points[i+1][0], FIVE_FIVE);
                    
                    this->control_Points[i+1].insert(this->control_Points[i+1].begin(), aide_point);
                    this->control_Points[i+1].insert(this->control_Points[i+1].begin(), this->control_Points[i][5]);

                    std::vector<Path> smooth_path1 = compute_Quintic(this->control_Points[i+1][0], this->control_Points[i+1][1],
                                                                    this->control_Points[i+1][2], this->control_Points[i+1][3],
                                                                    this->control_Points[i+1][4], this->control_Points[i+1][5]);

                    smooth_path.reserve(smooth_path.size() + smooth_path1.size());
                    smooth_path.insert(smooth_path.end(), smooth_path1.begin(), smooth_path1.end());
                }
                // zuihouyiduan you sanjie pinjie
                else if (i == this->number_of_line-2) {
                    XY aide_point = compute_aide_point(this->control_Points[i][3], this->control_Points[i][4], 
                                                        this->control_Points[i][5], this->control_Points[i+1][0], FIVE_THREE);
                    
                    this->control_Points[i+1].insert(this->control_Points[i+1].begin(), aide_point);
                    this->control_Points[i+1].insert(this->control_Points[i+1].begin(), this->control_Points[i][5]);

                    std::vector<Path> smooth_path1 = compute_Cubic(this->control_Points[i+1][0], this->control_Points[i+1][1],
                                                                    this->control_Points[i+1][2], this->control_Points[i+1][3]);
        
                    smooth_path.reserve(smooth_path.size() + smooth_path1.size());
                    smooth_path.insert(smooth_path.end(), smooth_path1.begin(), smooth_path1.end());
                }
            }
            break;
        }

        case FIVE_FIVE:{
            
            for (int i =0; i < this->number_of_line-1; ++i) {

                XY aide_point = compute_aide_point(this->control_Points[i][3], this->control_Points[i][4], 
                                                        this->control_Points[i][5], this->control_Points[i+1][0], FIVE_FIVE);
                    
                this->control_Points[i+1].insert(this->control_Points[i+1].begin(), aide_point);
                this->control_Points[i+1].insert(this->control_Points[i+1].begin(), this->control_Points[i][5]);

                std::vector<Path> smooth_path1 = compute_Quintic(this->control_Points[i+1][0], this->control_Points[i+1][1],
                                                                this->control_Points[i+1][2], this->control_Points[i+1][3],
                                                                this->control_Points[i+1][4], this->control_Points[i+1][5]);

                smooth_path.reserve(smooth_path.size() + smooth_path1.size());
                smooth_path.insert(smooth_path.end(), smooth_path1.begin(), smooth_path1.end());
            }
            break;
        }
        
        default:
            break;
        }
    }
    return smooth_path;
};


void Bezier::showpoints() {
    for (std::vector< std::vector<XY> >::iterator it = this->control_Points.begin();
            it!= this->control_Points.end();
            ++it) {
        for (std::vector<XY>::iterator it1 = it->begin();
            it1!=it->end();
            ++it1) {
            std::cout<< it1->m_X << " " << it1->m_Y << "  ";  
        }
        std::cout<< '\n';
    }
    std::cout<<"number of vec" << this->control_Points.size() << '\n';

}