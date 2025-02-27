/* This file contains a simple User STI tire model to VS solvers. 

   Copyright 1997 - 2015. Mechanical Simulation Corporation. All rights reserved.

   09-16-15 Y. Watanabe: Newly created.
*/

//#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vs_deftypes.h"
#include "simple_sti_tire.h"

#include <windows.h>

// Essential elements from Dyvars as generated by VS Lisp
typedef struct {
  vs_real c[6], s[6], z[68], q[6], r, dzdx, dzdy, zgnd, mu, comp, 
    r_free, kt, cfx, cfy, cmx, cmz, alpha, kappa, vx, vy, gamma,
    vlow_drift, vlow_slip, lrelax_x, lrelax_y,
    stateKappa, stateKappaDeriv, stateTana, stateTanaDeriv,
    fx, fy, fz, mx, my, mz;
} TireSTI;

static TireSTI **sTireVar;
static int sTiresN = 24;
#define READ_BUFFER_LEN 80

void simple_sti(
  void(__cdecl *message)(
  char   *msg),
  int    *iswtch,
  int    *jobflg,
  int    *idtyre,
  double *time,
  double  dis[3],
  double  tramat[9],  // 9-element vector passed as Fortran 3x3 matrix
  double *angtwc,
  double  vel[3],
  double  omega[3],
  double *omegar,
  int    *ndeqvr,
  double deqvar[],
  int    *ntypar,
  double  typarr[],
  int    *nchtds,
  char    chtdst[],
  int    *len_chtdst,
  void(__cdecl *road)(
  int    *idtyre,
  double dis[2],
  double *z,
  double dz[2],
  double *mu,
  int *ierr),
  int    *idroad,
  int    *nropar,
  double  ropar[],
  int    *nchrds,
  char    chrdst[],
  int    *len_chrdst,
  double  force[3],
  double  torque[3],
  double *deqini,
  double *deqder,
  char    tyrmod[256],
  int    *len_tyrmod,
  int    *nvars,
  double  varinf[],
  int    *nwork,
  double  wrkarr[],
  int    *niwork,
  int     iwrkar[],
  int    *ierr){

  vs_real yaw = 0, pitch = 0, roll = 0, x_gnd, y_gnd, pgnd, rgnd, dz[2], gnd[2];
  int i;
  static char str[READ_BUFFER_LEN];

  if (*idtyre > sTiresN) {
    *ierr = 3;
    return;
  }

  pitch = -asin(tramat[2]);
  roll = atan2(tramat[5],tramat[8]);
  yaw = atan2(tramat[1],tramat[0]);

  switch (*jobflg) {
  case 0: case 5: {
    TireSTI *var = sTireVar[*idtyre - 1];

    get_ctc_xy(*idtyre, dis[0], dis[1], dis[2], yaw, pitch, roll, var->r, var->dzdx,
    var->dzdy, &x_gnd, &y_gnd);

    gnd[0] = x_gnd;
    gnd[1] = y_gnd;

    if (!*jobflg){
      road(idtyre, gnd, &var->zgnd, dz, &var->mu, ierr);

      var->dzdx = dz[0];
      var->dzdy = dz[1];
    }

    get_ctc_kin(*idtyre, vel[0], vel[1], vel[2], omega[0], omega[1], omega[2], var->dzdx, var->dzdy,
      var->r, &var->gamma, &var->r, &var->vx, &var->vy, &x_gnd, &y_gnd, &var->zgnd, &pgnd, &rgnd);

    var->stateKappa = deqvar[0];
    var->stateTana = deqvar[1];

    sCalcTireCtcForces(*idtyre, *omegar);

    deqder[0] = var->stateKappaDeriv;
    deqder[1] = var->stateTanaDeriv;

    ctc_transform(*idtyre, var->r, var->dzdx, var->dzdy, var->fx, var->fy,
      var->fz, var->mx, var->my, var->mz,
      &force[0], &force[1], &force[2], &torque[0], &torque[1], &torque[2]);

    varinf[0] = var->fx;
    varinf[1] = var->fy;
    varinf[2] = var->fz;
    varinf[3] = var->mx;
    varinf[4] = var->my;
    varinf[5] = var->mz;
    varinf[6] = varinf[50] = var->alpha;
    varinf[7] = varinf[51] = var->kappa;
    varinf[8] = var->gamma;
    varinf[16] = var->mu;
    varinf[17] = var->mu;
    varinf[43] = var->comp;
    varinf[45] = var->vx;
    varinf[46] = var->vy;
    varinf[48] = var->r_free;
    varinf[52] = var->r;
    varinf[65] = x_gnd;
    varinf[66] = y_gnd;
    varinf[67] = var->zgnd;
    varinf[68] = -var->dzdx;
    varinf[69] = -var->dzdy;
    varinf[70] = 1.0;
    break;
  }
  case 1:
    sTireVar = (TireSTI **)calloc(sTiresN, sizeof(TireSTI *));
    for (i = 0; i < sTiresN; i++)
      sTireVar[i] = (TireSTI *)calloc(1, sizeof(TireSTI));

    message("***************************************************");
    message("*** Simple linear tire model with STI format    ***");
    message("*** Made by Yukio Watanabe - December 21, 2017   ***");
    message("*************************************************** \n");
    message("Tire model is initialized... \n");
    break;

  case 2:{
    TireSTI *var = sTireVar[*idtyre - 1];

    var->r_free = var->r = typarr[0];
    var->kt = typarr[1];
    var->cfx = typarr[2];
    var->cfy = typarr[3];
    var->cmx = typarr[4];
    var->cmz = typarr[5];
    var->vlow_drift = typarr[6];
    var->vlow_slip = typarr[7];
    var->lrelax_x = typarr[8];
    var->lrelax_y = typarr[9];

    message("Each tire is initialized...");
    sprintf(str, "Tire ID: %i", *idtyre);
    message(str);
    sprintf(str, "%.4f: Free rolling radius (m)", typarr[0]);
    message(str);
    sprintf(str, "%.1f: Vertical stiffness (N/m)", typarr[1]);
    message(str);
    sprintf(str, "%.1f: Longitudinal slip stiffness (N/1)", typarr[2]);
    message(str);
    sprintf(str, "%.1f: Lateral cornering stiffness (N/rad)", typarr[3]);
    message(str);
    sprintf(str, "%.1f: Overturning stiffness (N-m/rad)", typarr[4]);
    message(str);
    sprintf(str, "%.1f: Aligning stiffness (N-m/rad)", typarr[5]);
    message(str);
    sprintf(str, "%.4f: Low speed limit: allow near-static Fx or Fy", typarr[6]);
    message(str);
    sprintf(str, "      : to avoid drift when stopped on a grade (m/s)");
    message(str);
    sprintf(str, "%.4f: Minimum Vx used in ODE for lagged slip (m/s)", typarr[7]);
    message(str);
    sprintf(str, "%.4f: Tire longitudinal relaxation length (m)", typarr[8]);
    message(str);
    sprintf(str, "%.4f: Tire lateral relaxation length (m)\n", typarr[9]);
    message(str);

    break;
  }
  case 99:
    if (sTireVar){
      for (i = 0; i < sTiresN; i++) 
        free(sTireVar[i]);
      free(sTireVar); sTireVar = NULL;
      message("Tire model is terminated...");
    }
    break;
  }
}


// utility to return vx in denominator for kappa
static vs_real sVxDenKappa(TireSTI *var, vs_real abs_vx, vs_real dv_rot) {
  vs_real vlow;
  vs_real dv = max(fabs(var->vy), max(abs_vx, dv_rot)) - var->vlow_drift;

  if (dv < 0.0) // when "at rest" don't use vlow_kappa
    return max(0.01, abs_vx);

  vlow = max(abs_vx, var->vlow_slip);
  if (dv >= var->vlow_drift)
    return vlow;
  return abs_vx + (vlow - abs_vx)*dv / var->vlow_drift; // transition
}

static vs_real sVxDenAlpha(TireSTI *var, vs_real abs_vx) {
  vs_real vlow;
  vs_real dv = max(fabs(var->vy), abs_vx) - var->vlow_drift;

  if (dv < 0.0) // when "at rest" don't use vlow_alpha
    return abs_vx;

  vlow = max(abs_vx, var->vlow_slip);
  if (dv >= var->vlow_drift)
    return vlow;
  return abs_vx + (vlow - abs_vx)*dv / var->vlow_drift; // transition
}

/* ---------------------------------------------------------------------------------
Calculate instant kappa, the derivative for lagged kappa (state variable used
by tire models), and possibly override lagged kappa.
--------------------------------------------------------------------------------- */
static void sCalcKappaAndDeriv(int id, vs_real w) {
  TireSTI *var = sTireVar[id - 1];
  vs_real abs_vx = fabs(var->vx),
    dv_rot = var->r_free*w - var->vx,
    vx_den = sVxDenKappa(var, abs_vx, dv_rot),
    kappa_safe = dv_rot / max(0.01, vx_den);

  // if user has set relaxation length to zero, use safe instant slip
  if (var->lrelax_x == 0.0) {
    var->stateKappa = kappa_safe;
    var->stateKappaDeriv = 0.0;
    return;
  }

  // ODE for kappa_lag
  var->stateKappaDeriv = (dv_rot - vx_den * var->stateKappa) / var->lrelax_x;
}

static void sCalcTanAlphaAndDeriv(int id) {
  TireSTI *var = sTireVar[id - 1];
  vs_real abs_vx = fabs(var->vx),
    vx_den = sVxDenAlpha(var, abs_vx),
    tan_alpha = var->vy / max(0.01, abs_vx);

  // if user has set relaxation length to zero, use instantaneous slip
  if (var->lrelax_y == 0.0) {
    var->stateTana = tan_alpha;
    var->stateTanaDeriv = 0.0;
    return;
  }

  // ODE for dtana_lag
  var->stateTanaDeriv = (var->vy - (var->stateTana)*vx_den) / var->lrelax_y;
}

static void sCalcTireCtcForces(int id, vs_real w) {
  TireSTI *var = sTireVar[id - 1];

  var->fx = var->fy = var->fz = 0.0;
  var->mx = var->my = var->mz = 0.0;
  
  // Fz calculation
  var->comp = var->r_free - var->r;
  var->fz = var->kt * var->comp;

  sCalcKappaAndDeriv(id, w);
  sCalcTanAlphaAndDeriv(id);
    
  if (var->fz < 0.01) {
    var->alpha = 0;
    var->kappa = 0;
    var->fz = 0;
    var->comp = 0;
  }
  else { // no internal tire model. Just calculate alpha and kappa, then return.
    vs_real fzmu = var->mu*var->fz; 
    
    var->kappa = var->stateKappa;
    var->alpha = atan(var->stateTana);

    if (var->mu > 0.0) {
      var->fx = max(-1 * fzmu, min(fzmu, var->cfx * var->kappa * var->mu));
      var->fy = max(-1 * fzmu, min(fzmu, var->cfy * var->alpha * var->mu));
      var->mz = var->cmz * var->alpha * var->mu;
      var->mx = var->cmx * var->gamma * var->mu;
    }
    else {
      var->fx = max(-1 * var->fz, min(var->fz, var->cfx * var->kappa));
      var->fy = max(-1 * var->fz, min(var->fz, var->cfy * var->alpha));
      var->mz = var->cmz * var->alpha;
      var->mx = var->cmx * var->gamma;
    }
  }
}

void get_ctc_xy(int id, vs_real xw0, vs_real yw0, vs_real zw0,
  vs_real yaw, vs_real pitch, vs_real roll,
  vs_real r_old_1, vs_real dzdx_old_1, vs_real dzdy_old_1,
  vs_real *xgnd_est_1, vs_real *ygnd_est_1)
{
  vs_real *c = sTireVar[id - 1]->c, *s = sTireVar[id - 1]->s, 
          *z = sTireVar[id - 1]->z, *q = sTireVar[id - 1]->q;

  s[3] = sin(yaw);
  s[4] = sin(pitch);
  s[5] = sin(roll);
  c[3] = cos(yaw);
  c[4] = cos(pitch);
  c[5] = cos(roll);

  q[0] = xw0;
  q[1] = yw0;
  q[2] = zw0;

  // Get estimate of X and Y at CTC for first tire
  z[0] = c[4] * s[5];
  z[1] = c[3] * c[5];
  z[2] = s[4] * s[5];
  z[3] = z[1] + z[2] * s[3];
  z[4] = c[5] * s[3];
  z[5] = z[4] - z[2] * c[3];
  z[6] = z[0] - z[3] * dzdy_old_1 + z[5] * dzdx_old_1;
  z[7] = z[0] * z[6];
  z[8] = z[3] * z[6] + dzdy_old_1;
  z[9] = -z[5] * z[6] + dzdx_old_1;
  z[10] = 1.0 / sqrt(1 - z[7] + z[8] * dzdy_old_1 + z[9] * dzdx_old_1);
  z[11] = z[10] * r_old_1;
  *xgnd_est_1 = q[0] + z[9] * z[11];
  *ygnd_est_1 = q[1] + z[8] * z[11];
}

void get_ctc_kin(int id, vs_real vwx, vs_real vwy, vs_real vwz,
  vs_real avwx, vs_real avwy, vs_real avwz,
  vs_real dzdx_1, vs_real dzdy_1, vs_real r_old_1,
  vs_real *gamma_1, vs_real *rnew_1,
  vs_real *vctcx_1, vs_real *vctcy_1,
  vs_real *xgnd_1, vs_real *ygnd_1, vs_real *zgnd_1,
  vs_real *pitch_gnd, vs_real *roll_gnd)
{
  vs_real *c = sTireVar[id - 1]->c, *s = sTireVar[id - 1]->s,
          *z = sTireVar[id - 1]->z, *q = sTireVar[id - 1]->q;
  vs_real zgnd_old_1;

  // zgnd_old_1 is zgnd based on initial estimate of radius R.
  zgnd_old_1 = -(-q[2] + z[10] * (1 - z[7])*r_old_1);
  z[12] = 1 + (dzdy_1*dzdy_1) + (dzdx_1*dzdx_1);
  z[13] = 1.0 / sqrt(z[12]);
  z[14] = z[0] - z[3] * dzdy_1 + z[5] * dzdx_1;
  *gamma_1 = asin(z[13] * z[14]);

  // Corrected Radius based on new info about zgnd_1
  z[15] = cos(*gamma_1);
  (*rnew_1) = r_old_1 + z[13] * (zgnd_old_1 - (*zgnd_1)) / z[15];

  // Update X, Y, Z at CTC
  z[16] = z[4] - s[5] * (dzdx_1*c[4] + c[3] * s[4]);
  z[17] = s[3] * s[4];
  z[18] = z[1] + (z[17] + dzdy_1*c[4])*s[5];
  z[19] = z[5] * dzdy_1 + z[3] * dzdx_1;
  z[20] = 1.0 / sqrt((z[16] * z[16]) + (z[18] * z[18]) + (z[19] * z[19]));
  z[21] = z[19] * z[20];
  z[22] = z[16] * z[20];
  z[23] = z[18] * z[20];
  z[24] = z[23] * dzdy_1 - z[22] * dzdx_1;
  z[25] = z[13] * z[24];
  z[26] = z[13] * (z[23] + z[21] * dzdx_1);
  z[27] = z[13] * (z[22] + z[21] * dzdy_1);
  z[28] = z[0] * z[14];
  z[29] = z[3] * z[14] + dzdy_1;
  z[30] = -z[5] * z[14] + dzdx_1;
  z[31] = 1 - z[28] + z[29] * dzdy_1 + z[30] * dzdx_1;
  z[32] = 1.0 / sqrt(z[31]);
  z[33] = z[32] * (*rnew_1);
  *xgnd_1 = q[0] + z[30] * z[33];
  *ygnd_1 = q[1] + z[29] * z[33];
  z[34] = 1 - z[28];
  (*zgnd_1) = -(-q[2] + z[32] * z[34] * (*rnew_1));

  // calculate Vx, Vy at CTC
  z[35] = -z[21] * s[4] + c[4] * (z[23] * c[3] + z[22] * s[3]);
  z[36] = z[0] * z[21] + z[3] * z[22] - z[5] * z[23];
  z[37] = c[4] * c[5];
  z[38] = -z[17] * c[5] + c[3] * s[5];
  z[39] = c[3] * c[5] * s[4] + s[3] * s[5];
  z[40] = z[21] * z[37] - z[22] * z[38] + z[23] * z[39];
  z[41] = c[3] * c[4] * avwx;
  z[42] = z[5] * avwy;
  z[43] = z[39] * avwz;
  z[44] = avwx*s[4];
  z[45] = c[5] * avwz + avwy*s[5];
  z[46] = z[45] * c[4];
  z[47] = z[41] + z[43] - z[42] - (z[44] - z[46])*dzdx_1;
  z[48] = c[4] * avwx*s[3];
  z[49] = z[3] * avwy;
  z[50] = z[38] * avwz;
  z[51] = (z[44] - z[46])*dzdy_1;
  z[52] = (z[41] + z[43] - z[42])*dzdy_1 - (z[48] + z[49] - z[50])*dzdx_1;

  // Calculate Vx at CTC
  *vctcx_1 = z[35] * vwx + z[36] * vwy + z[40] * vwz - z[32] * (*rnew_1)*(-z[22] * z[47]
    - z[21] * z[52] + z[23] * (-z[50] - z[51] + z[48] + z[49]) + z[14]
    * (-z[40] * avwx + z[35] * avwz));
  z[53] = -z[25] * s[4] + c[4] * (-z[27] * c[3] + z[26] * s[3]);
  z[54] = z[0] * z[25] + z[3] * z[26] + z[5] * z[27];
  z[55] = -(-z[25] * z[37] + z[26] * z[38] + z[27] * z[39]);

  // Calculate Vy at CTC
  *vctcy_1 = -(-z[53] * vwx - z[54] * vwy - z[55] * vwz - z[32] * (*rnew_1)*(z[26] * z[47] +
    z[25] * z[52] + z[27] * (-z[50] - z[51] + z[48] + z[49]) - z[14]
    * (-z[55] * avwx + z[53] * avwz)));

  // Calculate pitch and roll at CTC (for animator)
  z[56] = asin(z[21]);
  *pitch_gnd = -z[56];
  z[57] = cos(z[56]);
  *roll_gnd = asin(z[25] / z[57]);
}

void ctc_transform(int id, vs_real rnew_1, vs_real dzdx_1, vs_real dzdy_1,
  vs_real fxctc_1, vs_real fyctc_1, vs_real fzctc_1,
  vs_real mxctc, vs_real myctc, vs_real mzctc,
  vs_real *fxw0, vs_real *fyw0, vs_real *fzw0,
  vs_real *mxw0, vs_real *myw0, vs_real *mzw0)
{
  vs_real *c = sTireVar[id - 1]->c, *s = sTireVar[id - 1]->s,
          *z = sTireVar[id - 1]->z;

  z[58] = s[4] + c[4] * (dzdx_1*c[3] + dzdy_1*s[3]);

  // Fx at wheel center
  *fxw0 = -z[13] * z[58] * fzctc_1 + z[35] * fxctc_1 + z[53] * fyctc_1;

  // Fy at wheel center
  *fyw0 = z[13] * z[14] * fzctc_1 + z[36] * fxctc_1 + z[54] * fyctc_1;
  z[59] = z[37] + z[38] * dzdy_1 - z[39] * dzdx_1;

  // Fz at wheel center
  *fzw0 = z[13] * z[59] * fzctc_1 + z[40] * fxctc_1 + z[55] * fyctc_1;

  // Add tire moments and moments of CTC forces applied to wheel center
  z[60] = 1.0 / sqrt(z[12] * z[31]);
  z[61] = -(z[23] * z[29] - z[22] * z[30])*fxctc_1 + (z[27] * z[29] +
    z[26] * z[30])*fyctc_1;
  z[62] = z[21] * z[29];
  z[63] = z[25] * z[29];
  z[64] = (z[62] + z[22] * z[34])*fxctc_1 + (z[63] + z[26] * z[34])*fyctc_1;
  z[65] = z[21] * z[30];
  z[66] = z[25] * z[30];
  z[67] = (z[65] + z[23] * z[34])*fxctc_1 - (z[27] * z[34] - z[66])*fyctc_1;

  // Mx at wheel center
  *mxw0 = -(z[13] * z[58] * mzctc - z[35] * mxctc - z[53] * myctc - rnew_1
    *(z[14] * z[60] * fzctc_1*(-z[19] * s[4] + c[4] * (z[18] * c[3] + z[16] * s[3]))
    + z[32] * (-z[61] * s[4] + c[4] * (z[64] * c[3] - z[67] * s[3]))));

  // My at wheel center
  *myw0 = z[13] * z[14] * mzctc + z[36] * mxctc + z[54] * myctc - z[32] * rnew_1*(z[5]
    * ((z[22] + z[62])*fxctc_1 + (z[26] + z[63])*fyctc_1) + z[3] * ((z[23] +
    z[65])*fxctc_1 - (z[27] - z[66])*fyctc_1) - z[0] * (-z[24] * fxctc_1 +
    (z[27] * dzdy_1 + z[26] * dzdx_1)*fyctc_1));

  // Mz at wheel center
  *mzw0 = z[13] * z[59] * mzctc + z[40] * mxctc + z[55] * myctc + rnew_1*(z[32]
    * (z[37] * z[61] + z[39] * z[64] + z[38] * z[67]) + z[14] * z[60]
    * (-z[16] * z[38] + z[19] * z[37] + z[18] * z[39])*fzctc_1);
}
