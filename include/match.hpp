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
  double lb1_tmp;
  double v_idx_0;
  double v_idx_1;
  double v_idx_2;
  double c;
  double vx[9];
  int i;
  signed char b_I[9];
  int k;
  double b;
  double R[9];
  double d;
  double a2ci[3];
  double b2r_idx_0;
  double v_tmp;
  double b_v_tmp;
  double b2r_idx_1;
  double b_vx[9];
  scale = 3.3121686421112381E-170;

  // la2=norm(a2);
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
    lb1_tmp = 1.0;
    b_scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    lb1_tmp = t * t;
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
    lb1_tmp = lb1_tmp * t * t + 1.0;
    b_scale = absxk;
  } else {
    t = absxk / b_scale;
    lb1_tmp += t * t;
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
    lb1_tmp = lb1_tmp * t * t + 1.0;
    b_scale = absxk;
  } else {
    t = absxk / b_scale;
    lb1_tmp += t * t;
  }

  la1 = scale * std::sqrt(la1);
  lb1_tmp = b_scale * std::sqrt(lb1_tmp);

  // lb2=norm(b2);
  v_idx_0 = (a1[1] * b1[2] - a1[2] * b1[1]) / la1 / lb1_tmp;
  v_idx_1 = (a1[2] * b1[0] - a1[0] * b1[2]) / la1 / lb1_tmp;
  v_idx_2 = (a1[0] * b1[1] - a1[1] * b1[0]) / la1 / lb1_tmp;
  scale = 3.3121686421112381E-170;
  absxk = std::abs(v_idx_0);
  if (absxk > 3.3121686421112381E-170) {
    b_scale = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    b_scale = t * t;
  }

  absxk = std::abs(v_idx_1);
  if (absxk > scale) {
    t = scale / absxk;
    b_scale = b_scale * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    b_scale += t * t;
  }

  absxk = std::abs(v_idx_2);
  if (absxk > scale) {
    t = scale / absxk;
    b_scale = b_scale * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    b_scale += t * t;
  }

  b_scale = scale * std::sqrt(b_scale);
  c = ((a1[0] * b1[0] + a1[1] * b1[1]) + a1[2] * b1[2]) / la1 / lb1_tmp;
  if (c > -0.999) {
    vx[0] = 0.0;
    vx[3] = -v_idx_2;
    vx[6] = v_idx_1;
    vx[1] = v_idx_2;
    vx[4] = 0.0;
    vx[7] = -v_idx_0;
    vx[2] = -v_idx_1;
    vx[5] = v_idx_0;
    vx[8] = 0.0;
    for (i = 0; i < 9; i++) {
      b_I[i] = 0;
    }

    b_I[0] = 1;
    b_I[4] = 1;
    b_I[8] = 1;
    for (i = 0; i < 3; i++) {
      for (k = 0; k < 3; k++) {
        int R_tmp;
        R_tmp = i + 3 * k;
        R[R_tmp] = (static_cast<double>(b_I[R_tmp]) + vx[R_tmp]) + ((vx[i] * vx
          [3 * k] + vx[i + 3] * vx[3 * k + 1]) + vx[i + 6] * vx[3 * k + 2]) / (c
          + 1.0);
      }
    }
  } else {
    if (a1[0] > 0.0) {
      v_idx_0 = -(a1[1] + a1[2]) / a1[0];
      v_idx_1 = 1.0;
      v_idx_2 = 1.0;
    } else if (a1[1] > 0.0) {
      v_idx_0 = 1.0;
      v_idx_1 = -(a1[0] + a1[2]) / a1[1];
      v_idx_2 = 1.0;
    } else {
      v_idx_0 = 1.0;
      v_idx_1 = 1.0;
      v_idx_2 = -(a1[0] + a1[1]) / a1[2];
    }

    scale = 3.3121686421112381E-170;
    absxk = std::abs(v_idx_0);
    if (absxk > 3.3121686421112381E-170) {
      la1 = 1.0;
      scale = absxk;
    } else {
      t = absxk / 3.3121686421112381E-170;
      la1 = t * t;
    }

    absxk = std::abs(v_idx_1);
    if (absxk > scale) {
      t = scale / absxk;
      la1 = la1 * t * t + 1.0;
      scale = absxk;
    } else {
      t = absxk / scale;
      la1 += t * t;
    }

    absxk = std::abs(v_idx_2);
    if (absxk > scale) {
      t = scale / absxk;
      la1 = la1 * t * t + 1.0;
      scale = absxk;
    } else {
      t = absxk / scale;
      la1 += t * t;
    }

    la1 = scale * std::sqrt(la1);
    v_idx_0 /= la1;
    v_idx_1 /= la1;
    v_idx_2 /= la1;
    vx[0] = 0.0;
    vx[3] = -v_idx_2;
    vx[6] = v_idx_1;
    vx[1] = v_idx_2;
    vx[4] = 0.0;
    vx[7] = -v_idx_0;
    vx[2] = -v_idx_1;
    vx[5] = v_idx_0;
    vx[8] = 0.0;
    for (i = 0; i < 9; i++) {
      b_I[i] = 0;
    }

    for (k = 0; k < 3; k++) {
      b_I[k + 3 * k] = 1;
      for (i = 0; i < 3; i++) {
        R[k + 3 * i] = (vx[k] * vx[3 * i] + vx[k + 3] * vx[3 * i + 1]) + vx[k +
          6] * vx[3 * i + 2];
      }
    }

    for (i = 0; i < 9; i++) {
      R[i] = (static_cast<double>(b_I[i]) + vx[i] * b_scale) + R[i] * (1.0 - c);
    }
  }

  b = 0.0;
  for (k = 0; k < 3; k++) {
    d = (a2[0] * R[k] + a2[1] * R[k + 3]) + a2[2] * R[k + 6];
    a2ci[k] = d;
    b += d * b1[k];
  }

  la1 = lb1_tmp * lb1_tmp;
  a2ci[0] -= b1[0] * b / la1;
  a2ci[1] -= b1[1] * b / la1;
  a2ci[2] -= b1[2] * b / la1;
  b = (b1[0] * b2[0] + b1[1] * b2[1]) + b1[2] * b2[2];
  scale = 3.3121686421112381E-170;
  b_scale = 3.3121686421112381E-170;
  d = b2[0] - b1[0] * b / la1;
  b2r_idx_0 = d;
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

  d = b2[1] - b1[1] * b / la1;
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

  d = b2[2] - b1[2] * b / la1;
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

  v_tmp = scale * std::sqrt(v_tmp);
  b_v_tmp = b_scale * std::sqrt(b_v_tmp);
  v_idx_0 = (a2ci[1] * d - a2ci[2] * b2r_idx_1) / b_v_tmp / v_tmp;
  v_idx_1 = (a2ci[2] * b2r_idx_0 - a2ci[0] * d) / b_v_tmp / v_tmp;
  v_idx_2 = (a2ci[0] * b2r_idx_1 - a2ci[1] * b2r_idx_0) / b_v_tmp / v_tmp;
  scale = 3.3121686421112381E-170;
  absxk = std::abs(v_idx_0);
  if (absxk > 3.3121686421112381E-170) {
    b = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    b = t * t;
  }

  absxk = std::abs(v_idx_1);
  if (absxk > scale) {
    t = scale / absxk;
    b = b * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    b += t * t;
  }

  absxk = std::abs(v_idx_2);
  if (absxk > scale) {
    t = scale / absxk;
    b = b * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    b += t * t;
  }

  b = scale * std::sqrt(b);
  la1 = b1[0] / lb1_tmp;
  b_scale = la1;
  c = la1 * v_idx_0;
  la1 = b1[1] / lb1_tmp;
  scale = la1;
  c += la1 * v_idx_1;
  la1 = b1[2] / lb1_tmp;
  c += la1 * v_idx_2;
  if (c > 0.0) {
    v_idx_0 = b_scale;
    v_idx_1 = scale;
    v_idx_2 = la1;
  } else {
    v_idx_0 = -b_scale;
    v_idx_1 = -scale;
    v_idx_2 = -la1;
  }

  c = ((a2ci[0] * b2r_idx_0 + a2ci[1] * b2r_idx_1) + a2ci[2] * d) / b_v_tmp /
    v_tmp;
  vx[0] = 0.0;
  vx[3] = -v_idx_2;
  vx[6] = v_idx_1;
  vx[1] = v_idx_2;
  vx[4] = 0.0;
  vx[7] = -v_idx_0;
  vx[2] = -v_idx_1;
  vx[5] = v_idx_0;
  vx[8] = 0.0;
  for (i = 0; i < 9; i++) {
    b_I[i] = 0;
  }

  for (k = 0; k < 3; k++) {
    b_I[k + 3 * k] = 1;
    for (i = 0; i < 3; i++) {
      b_vx[k + 3 * i] = (vx[k] * vx[3 * i] + vx[k + 3] * vx[3 * i + 1]) + vx[k +
        6] * vx[3 * i + 2];
    }
  }

  for (i = 0; i < 9; i++) {
    vx[i] = (static_cast<double>(b_I[i]) + vx[i] * b) + b_vx[i] * (1.0 - c);
  }

  for (i = 0; i < 3; i++) {
    d = vx[i + 3];
    la1 = vx[i + 6];
    for (k = 0; k < 3; k++) {
      o[i + 3 * k] = (vx[i] * R[3 * k] + d * R[3 * k + 1]) + la1 * R[3 * k + 2];
    }
  }
}

void mproduct(const double A[9], const double X[3], double o[3])
{
  for (int i = 0; i < 3; i++) {
    o[i] = (A[i] * X[0] + A[i + 3] * X[1]) + A[i + 6] * X[2];
  }
}

bool ismatch(const double A[9], const double X[3],const double center[3],const double *target,double error){
    double a[3],c[3];
    a[0]=X[0]-center[0];
    a[1]=X[1]-center[1];
    a[2]=X[2]-center[2];
    mproduct(A,a,c);
    if (fabs(c[0]-target[0])<error && fabs(c[1]-target[1])<error && fabs(c[2]-target[2])<error){
        return true;
    }
    return false;
}



// int main(){
//     double a1[]={1,2,1},a2[]={0,1,2};
//     double b1[]={2,1,1},b2[]={1,2,0};
//     double o[9];
//     generate_rotation(a1,a2,b1,b2,o);
//     double c[3];
//     mproduct(o,a1,c);
//     std::cout<<c[0]<<" "<<c[1]<<"  "<<c[2]<<std::endl;
//     for (int i = 0; i < 9; i++)
//     {
//         std::cout<<o[i]<<std::endl;
//     }
    
//     return 0;
// }