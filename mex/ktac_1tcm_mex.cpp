#include "mex.h"
#include "kmaplib.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// This file implements the computation of the time activity curve (TAC) and its
// Jacobian for a one-tissue kinetic model (1TCM) within the MATLAB environment.
//
// Usage:
// ktac_1tcm(par, scant, blood, wblood, dk, td)
//
//
// This will produce a MEX file named 'ktac_1tcm', which you can call from MATLAB 
// as ktac_1tcm(...) with the same arguments as described above.
//
// Input parameters:
// - par: Model parameters.
// - scant: Scan time data.
// - blood: Blood data.
// - wblood: Whole blood data.
// - dk: Decay constant.
// - td: Time duration for the scan.
//
// Output:
// - c: Computed time activity curve (TAC).
// - s: Jacobian matrix (if requested).
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {   

    double *par, *scant, *b, *wb;
    double dk, td;
    int num_par, num_vox, num_frm; 
    double *c, *s;
    int psens[4] = {1, 1, 1, 1};  // Default sensitivity for 3 parameters
  
    // Retrieve input arguments from MATLAB
    par = mxGetPr(prhs[0]);  
    num_par = (int)mxGetM(prhs[0]);
    num_vox = (int)mxGetN(prhs[0]);
    scant = mxGetPr(prhs[1]);      
    num_frm = (int)mxGetM(prhs[1]);
    b = mxGetPr(prhs[2]); 
    wb = mxGetPr(prhs[3]); 
    dk = mxGetScalar(prhs[4]);
    td = mxGetScalar(prhs[5]);
  
    // Allocate memory for output
    plhs[0] = mxCreateDoubleMatrix(num_frm, num_vox, mxREAL);
    c = mxGetPr(plhs[0]);
    if (nlhs > 1) {
        plhs[1] = mxCreateDoubleMatrix(num_frm, num_par, mxREAL);
        s = mxGetPr(plhs[1]);
    }
    
    // Perform voxel-wise computation
    if (nlhs == 1) {
        kconv_1tcm_tac(par, dk, scant, td, b, wb, num_frm, num_vox, c);
    } else if (nlhs > 1) {
        kconv_1tcm_jac(par, dk, scant, td, b, wb, num_frm, num_vox, c, psens, s);
    }
}
