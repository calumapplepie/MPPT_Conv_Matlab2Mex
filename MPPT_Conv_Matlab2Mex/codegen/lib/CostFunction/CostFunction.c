/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: CostFunction.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 21-May-2024 22:00:04
 */

/* Include Files */
#include "CostFunction.h"
#include "expm.h"
#include <math.h>

/* Function Definitions */
/*
 * function [Sa, Sb, Sc, J] = fcn(Vdc, sys, Ref, If, Vc, Io, mode)
 *
 * Arguments    : double Vdc
 *                const double sys[5]
 *                const double Ref[2]
 *                const double If[2]
 *                const double Vc[2]
 *                const double Io[2]
 *                double mode
 *                double *Sa
 *                double *Sb
 *                double *Sc
 *                double *J
 * Return Type  : void
 */
void CostFunction(double Vdc, const double sys[5], const double Ref[2],
                  const double If[2], const double Vc[2], const double Io[2],
                  double mode, double *Sa, double *Sb, double *Sc, double *J)
{
  static const signed char states[24] = {0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1,
                                         1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1};
  creal_T Vinv[8];
  double A[4];
  double Ad[4];
  double Bd[4];
  double b_I[4];
  double J_out;
  double a21;
  double a22;
  double d;
  double d1;
  double d10;
  double d2;
  double d3;
  double d4;
  double d5;
  double d6;
  double d7;
  double d8;
  double d9;
  double v2_tmp_im;
  int VSI;
  int r1;
  int r2;
  int xout;
  /* 'CostFunction:3' Sa = 0; */
  /* 'CostFunction:4' Sb = 0; */
  /* 'CostFunction:5' Sc = 0; */
  /* 'CostFunction:7' xout  = 0; */
  xout = -1;
  /* 'CostFunction:8' J     = 0; */
  *J = 0.0;
  /* 'CostFunction:9' J_out = 1e9; */
  J_out = 1.0E+9;
  /* 'CostFunction:11' Ts  = sys(1); */
  /* 'CostFunction:12' Lf  = sys(2); */
  /* 'CostFunction:13' Cf  = sys(3); */
  /* 'CostFunction:14' Rf  = sys(4); */
  /*  Inductor ESR  */
  /* 'CostFunction:15' Rc  = sys(5); */
  /* 'CostFunction:17' refa = Ref(1); */
  /* 'CostFunction:17' refb = Ref(2); */
  /* 'CostFunction:18' Vca   = Vc(1); */
  /* 'CostFunction:18' Vcb = Vc(2); */
  /* 'CostFunction:19' Ioa   = Io(1); */
  /* 'CostFunction:19' Iob = Io(2); */
  /* 'CostFunction:20' ILa   = If(1); */
  /* 'CostFunction:20' ILb = If(2); */
  /* 'CostFunction:22' VSI = 0; */
  VSI = 0;
  /* 'CostFunction:22' CSI = 0; */
  /*  Initialize Modes for Cost Function */
  /* 'CostFunction:23' Mode = VSI; */
  /*  Select Operation Mode */
  /* 'CostFunction:25' if mode == 0 */
  if (mode == 0.0) {
    /* 'CostFunction:26' VSI = 1; */
    VSI = 1;
    /* 'CostFunction:26' CSI = 0; */
  }
  /*  disp(CSI); disp(VSI); */
  /*  A = [-R/L -1/L;1/Cf 0   ;  ... */
  /*       Rc/Cf       0        -Rc/Cf;  ... */
  /*       0        1/Lt    -Rt/Lt ]; */
  /*   */
  /*  B = [1/Lf; 0; 0]; */
  /*   */
  /*  D = [0 0;0 -1/Lf]; */
  /* 'CostFunction:39' A = [-Rf/Lf -1/Lf;1/Cf 0]; */
  A[0] = -sys[3] / sys[1];
  A[2] = -1.0 / sys[1];
  A[1] = 1.0 / sys[2];
  A[3] = 0.0;
  /* 'CostFunction:40' B = [1/Lf 0;0 -1/Cf]; */
  /* 'CostFunction:42' I  = eye(2); */
  /* 'CostFunction:43' Ad = expm(A.*Ts); */
  Bd[0] = A[0] * sys[0];
  Bd[1] = sys[0] * A[1];
  Bd[2] = sys[0] * A[2];
  Bd[3] = 0.0;
  expm(Bd, Ad);
  /* 'CostFunction:44' Bd = A\(Ad - I); */
  b_I[0] = Ad[0] - 1.0;
  b_I[1] = Ad[1];
  b_I[2] = Ad[2];
  b_I[3] = Ad[3] - 1.0;
  if (fabs(A[1]) > fabs(A[0])) {
    r1 = 1;
    r2 = 0;
  } else {
    r1 = 0;
    r2 = 1;
  }
  a21 = A[r2] / A[r1];
  a22 = A[r2 + 2] - a21 * A[r1 + 2];
  d = (b_I[r2 + 2] - b_I[r1 + 2] * a21) / a22;
  /*  Dd = inv(A)*(Ad - I)*D; */
  /*  Voltage vectors */
  /* 'CostFunction:48' v0 =  0; */
  /* 'CostFunction:49' v1 =  2/3 *Vdc; */
  /* 'CostFunction:50' v2 =  1/3*Vdc + 1j*sqrt(3)/3*Vdc; */
  v2_tmp_im = Vdc * 0.57735026918962573;
  /* 'CostFunction:51' v3 = -1/3*Vdc + 1j*sqrt(3)/3*Vdc; */
  /* 'CostFunction:52' v4 = -2/3*Vdc; */
  /* 'CostFunction:53' v5 = -1/3*Vdc - 1j*sqrt(3)/3*Vdc; */
  /* 'CostFunction:54' v6 =  1/3*Vdc - 1j*sqrt(3)/3*Vdc; */
  /* 'CostFunction:55' v7 =  0; */
  /* 'CostFunction:56' Vinv = [v0 v1 v2 v3 v4 v5 v6 v7]; */
  Vinv[0].re = 0.0;
  Vinv[0].im = 0.0;
  Vinv[1].re = 0.66666666666666663 * Vdc;
  Vinv[1].im = 0.0;
  Vinv[2].re = 0.33333333333333331 * Vdc;
  Vinv[2].im = v2_tmp_im;
  Vinv[3].re = -0.33333333333333331 * Vdc;
  Vinv[3].im = v2_tmp_im;
  Vinv[4].re = -0.66666666666666663 * Vdc;
  Vinv[4].im = 0.0;
  Vinv[5].re = -0.33333333333333331 * Vdc;
  Vinv[5].im = 0.0 - v2_tmp_im;
  Vinv[6].re = 0.33333333333333331 * Vdc;
  Vinv[6].im = 0.0 - v2_tmp_im;
  Vinv[7].re = 0.0;
  Vinv[7].im = 0.0;
  /* 'CostFunction:58' states = [0 0 0; 1 0 0; 1 1 0; 0 1 0; ... */
  /* 'CostFunction:59'           0 1 1; 0 0 1; 1 0 1; 1 1 1]; */
  /*  Avoid Error xout = 0 out of range [1,8] */
  /*  if xout == 0, xout = 1; end */
  /* 'CostFunction:64' for i = 1:8 */
  d1 = Ref[0];
  d2 = Ref[1];
  d3 = If[0];
  d4 = If[1];
  d5 = Ad[1];
  d6 = Ad[3];
  d7 = Vc[0];
  d8 = Vc[1];
  a22 = (b_I[r2] - b_I[r1] * a21) / a22;
  d9 = Io[0];
  d10 = Io[1];
  for (r1 = 0; r1 < 8; r1++) {
    /*  i-th voltage vector for current prediction */
    /* 'CostFunction:67' Vinva = real(Vinv(i)); */
    /* 'CostFunction:68' Vinvb = imag(Vinv(i)); */
    /*  For alpha  */
    /*  x= Ad*[ILa;Vca;Ioa] + Bd*[Vinva]; + Dd*[Voa]; */
    /* 'CostFunction:72' x= Ad*[ILa;Vca] + Bd*[Vinva;Ioa]; */
    /* 'CostFunction:73' V_k1a = x(2); */
    /*  For Beta  */
    /*  y= Ad*[ILb;Vcb;Iob] + Bd*[Vinvb] + Dd*[Vob]; */
    /* 'CostFunction:77' y= Ad*[ILb;Vcb] + Bd*[Vinvb;Iob]; */
    /* 'CostFunction:78' V_k1b = y(2); */
    /* 'CostFunction:82' J = VSI*((refa - V_k1a)^2 + (refb - V_k1b)^2); */
    v2_tmp_im = d1 - ((d3 * d5 + d7 * d6) + (a22 * Vinv[r1].re + d * d9));
    a21 = d2 - ((d5 * d4 + d8 * d6) + (a22 * Vinv[r1].im + d * d10));
    *J = (double)VSI * (v2_tmp_im * v2_tmp_im + a21 * a21);
    /* 'CostFunction:84' if ( J < J_out) */
    if (*J < J_out) {
      /* 'CostFunction:85' J_out = J; */
      J_out = *J;
      /* 'CostFunction:86' xout = i; */
      xout = r1;
    }
  }
  /* 'CostFunction:91' Sa = states(xout,1); */
  *Sa = states[xout];
  /* 'CostFunction:92' Sb = states(xout,2); */
  *Sb = states[xout + 8];
  /* 'CostFunction:93' Sc = states(xout,3); */
  *Sc = states[xout + 16];
}

/*
 * File trailer for CostFunction.c
 *
 * [EOF]
 */
