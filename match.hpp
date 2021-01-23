#include<iostream>

//叉乘
void cross(const double inputArg1[3], const double inputArg2[3], double
              outputArg[3])
{
  outputArg[0] = inputArg1[1] * inputArg2[2] - inputArg1[2] * inputArg2[1];
  outputArg[1] = inputArg1[2] * inputArg2[0] - inputArg1[0] * inputArg2[2];
  outputArg[2] = inputArg1[0] * inputArg2[1] - inputArg1[1] * inputArg2[0];
}


//可以被重合,修改M,a1,a2,b1,b2,1x3,M 1x9
#include <cmath>
#include<iostream>
// Function Definitions

//
// UNTITLED2 此处显示有关此函数的摘要
//    将a1，a2,转动到与b1,b2重合的矩阵
// Arguments    : const double a1[3]
//                const double a2[3]
//                const double b1[3]
//                const double b2[3]
//                double o[9]
// Return Type  : void
//
void generate_rotation(const double a1[3], const double a2[3], const double b1[3], const double
       b2[3], double o[9])
{
  double scale;
  double b_scale;
  double absxk;
  double t;
  double la1;
  double lb1;
  double v_idx_2;
  double c;
  double vx[9];
  int i;
  signed char R_tmp[9];
  int k;
  double d;
  double a2ci[3];
  int i1;
  double R[9];
  double v_tmp;
  double b_v_tmp;
  double b2r_idx_1;
  double b_R_tmp[9];
  scale = 3.3121686421112381E-170;
  b_scale = 3.3121686421112381E-170;
  absxk = std::abs(a1[0]);
  if (absxk > 3.3121686421112381E-170) {
    la1 = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    la1 = t * t;
  }

  absxk = std::abs(b1[0]);
  if (absxk > 3.3121686421112381E-170) {
    lb1 = 1.0;
    b_scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    lb1 = t * t;
  }

  absxk = std::abs(a1[1]);
  if (absxk > scale) {
    t = scale / absxk;
    la1 = la1 * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    la1 += t * t;
  }

  absxk = std::abs(b1[1]);
  if (absxk > b_scale) {
    t = b_scale / absxk;
    lb1 = lb1 * t * t + 1.0;
    b_scale = absxk;
  } else {
    t = absxk / b_scale;
    lb1 += t * t;
  }

  absxk = std::abs(a1[2]);
  if (absxk > scale) {
    t = scale / absxk;
    la1 = la1 * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    la1 += t * t;
  }

  absxk = std::abs(b1[2]);
  if (absxk > b_scale) {
    t = b_scale / absxk;
    lb1 = lb1 * t * t + 1.0;
    b_scale = absxk;
  } else {
    t = absxk / b_scale;
    lb1 += t * t;
  }

  la1 = scale * std::sqrt(la1);
  lb1 = b_scale * std::sqrt(lb1);
  b_scale = (a1[1] * b1[2] - a1[2] * b1[1]) / la1 / lb1;
  scale = (a1[2] * b1[0] - a1[0] * b1[2]) / la1 / lb1;
  v_idx_2 = (a1[0] * b1[1] - a1[1] * b1[0]) / la1 / lb1;
  c = ((a1[0] * b1[0] + a1[1] * b1[1]) + a1[2] * b1[2]) / la1 / lb1;
  vx[0] = 0.0;
  vx[3] = -v_idx_2;
  vx[6] = scale;
  vx[1] = v_idx_2;
  vx[4] = 0.0;
  vx[7] = -b_scale;
  vx[2] = -scale;
  vx[5] = b_scale;
  vx[8] = 0.0;
  for (i = 0; i < 9; i++) {
    R_tmp[i] = 0;
  }

  R_tmp[0] = 1;
  R_tmp[4] = 1;
  R_tmp[8] = 1;
  la1 = 0.0;
  for (k = 0; k < 3; k++) {
    d = 0.0;
    for (i = 0; i < 3; i++) {
      i1 = k + 3 * i;
      v_idx_2 = (static_cast<double>(R_tmp[i1]) + vx[i1]) + ((vx[k] * vx[3 * i]
        + vx[k + 3] * vx[3 * i + 1]) + vx[k + 6] * vx[3 * i + 2]) / (c + 1.0);
      R[i1] = v_idx_2;
      d += a2[i] * v_idx_2;
    }

    a2ci[k] = d;
    la1 += d * b1[k];
  }

  v_idx_2 = lb1 * lb1;
  a2ci[0] -= b1[0] * la1 / v_idx_2;
  a2ci[1] -= b1[1] * la1 / v_idx_2;
  a2ci[2] -= b1[2] * la1 / v_idx_2;
  la1 = (b1[0] * b2[0] + b1[1] * b2[1]) + b1[2] * b2[2];
  scale = 3.3121686421112381E-170;
  b_scale = 3.3121686421112381E-170;
  d = b2[0] - b1[0] * la1 / v_idx_2;
  lb1 = d;
  absxk = std::abs(d);
  if (absxk > 3.3121686421112381E-170) {
    v_tmp = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    v_tmp = t * t;
  }

  absxk = std::abs(a2ci[0]);
  if (absxk > 3.3121686421112381E-170) {
    b_v_tmp = 1.0;
    b_scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    b_v_tmp = t * t;
  }

  c = a2ci[0] * d;
  d = b2[1] - b1[1] * la1 / v_idx_2;
  b2r_idx_1 = d;
  absxk = std::abs(d);
  if (absxk > scale) {
    t = scale / absxk;
    v_tmp = v_tmp * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    v_tmp += t * t;
  }

  absxk = std::abs(a2ci[1]);
  if (absxk > b_scale) {
    t = b_scale / absxk;
    b_v_tmp = b_v_tmp * t * t + 1.0;
    b_scale = absxk;
  } else {
    t = absxk / b_scale;
    b_v_tmp += t * t;
  }

  c += a2ci[1] * d;
  d = b2[2] - b1[2] * la1 / v_idx_2;
  absxk = std::abs(d);
  if (absxk > scale) {
    t = scale / absxk;
    v_tmp = v_tmp * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    v_tmp += t * t;
  }

  absxk = std::abs(a2ci[2]);
  if (absxk > b_scale) {
    t = b_scale / absxk;
    b_v_tmp = b_v_tmp * t * t + 1.0;
    b_scale = absxk;
  } else {
    t = absxk / b_scale;
    b_v_tmp += t * t;
  }

  c += a2ci[2] * d;
  v_tmp = scale * std::sqrt(v_tmp);
  b_v_tmp = b_scale * std::sqrt(b_v_tmp);
  b_scale = (a2ci[1] * d - a2ci[2] * b2r_idx_1) / b_v_tmp / v_tmp;
  scale = (a2ci[2] * lb1 - a2ci[0] * d) / b_v_tmp / v_tmp;
  v_idx_2 = (a2ci[0] * b2r_idx_1 - a2ci[1] * lb1) / b_v_tmp / v_tmp;
  c = c / b_v_tmp / v_tmp;
  vx[0] = 0.0;
  vx[3] = -v_idx_2;
  vx[6] = scale;
  vx[1] = v_idx_2;
  vx[4] = 0.0;
  vx[7] = -b_scale;
  vx[2] = -scale;
  vx[5] = b_scale;
  vx[8] = 0.0;
  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < 3; i1++) {
      k = i + 3 * i1;
      b_R_tmp[k] = (static_cast<double>(R_tmp[k]) + vx[k]) + ((vx[i] * vx[3 * i1]
        + vx[i + 3] * vx[3 * i1 + 1]) + vx[i + 6] * vx[3 * i1 + 2]) / (c + 1.0);
    }

    d = b_R_tmp[i + 3];
    v_idx_2 = b_R_tmp[i + 6];
    for (i1 = 0; i1 < 3; i1++) {
      o[i + 3 * i1] = (b_R_tmp[i] * R[3 * i1] + d * R[3 * i1 + 1]) + v_idx_2 *
        R[3 * i1 + 2];
    }
  }
}

void mproduct(const double A[9], const double X[3], double o[3])
{
  for (int i = 0; i < 3; i++) {
    o[i] = (A[i] * X[0] + A[i + 3] * X[1]) + A[i + 6] * X[2];
  }
}

bool ismatch(const double A[9], const double X[3],const double center[3],const double target,double error){
    double a[3],b[3],c[3];
    a[0]=X[0]-center[0];
    a[1]=X[1]-center[1];
    a[2]=X[2]-center[2];
    b[0]=X[0]-center[0];
    b[1]=X[1]-center[1];
    b[2]=X[2]-center[2];
    mproduct(A,a,c);
    if (fabs(c[0]-b[0])<error && fabs(c[1]-b[1])<error && fabs(c[2]-b[2])<error){
        return true;
    }
    return false;
}



// int main(){
//     double a1[]={1,2,1},a2[]={0,1,2};
//     double b1[]={2,1,1},b2[]={1,2,0};
//     double o[9];
//     M(a1,a2,b1,b2,o);
//     for (int i = 0; i < 9; i++)
//     {
//         std::cout<<o[i]<<std::endl;
//     }
    
//     return 0;
// }