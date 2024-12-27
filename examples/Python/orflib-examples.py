# orflib library examples

#%%
import orflib as orf
import numpy as np
import os

ver = orf.version()
print(f'orflib version: {ver}')
pid = os.getpid()
print(f'pid: {pid}')

#%%
# function group 0
print('=================')
print('Utility functions')

#MatrixEcho
inmat = np.array(np.arange(1.0, 21, 2)).reshape([5, 2])
outmat = orf.matrixEcho(inmat)
print('Matrix echo')
print(inmat)
print(outmat)

#CholDcmp
inmat = np.array([[4, 12, -16], [12, 37, -43], [-16, -43, 98]], dtype = 'float')
lmat = orf.cholDcmp(inmat)
outmat = np.dot(lmat, lmat.T)
print('Cholesky decomposition')
print(inmat)
print(outmat)

#EigenSym
inmat = np.array([[4, 12, -16], [12, 37, -43], [-16, -43, 98]], dtype = 'float')
eigen = orf.eigenSym(inmat)
print('Eigenvalues and eigenvectors of symmetric matrix')
print('Eigenvalues = ', eigen['Eigenvals'])
print('Eigenvectors = ')
print(eigen['Eigenvecs'])

#SpecTrunc
incormat = np.array([[1, 0.2, -0.8], [0.2, 1, 0.5], [-0.8, 0.5, 1]])
print('Spectral truncation of correlation matrix')
print(incormat)
print(orf.specTrunc(incormat))

#PPolyEval
xvec = np.arange(0, 5.5, 0.2)
yval = orf.ppolyEval(np.arange(1, 6), np.arange(-10, 15, 5), 1, xvec, 0)
yder = orf.ppolyEval(np.arange(1, 6), np.arange(-10, 15, 5), 1, xvec, 1)
yint = orf.ppolyIntegral(np.arange(1, 6), np.arange(-10, 15, 5), 1, xvec[0], xvec)
print('Piecewise polynomial')
print(f'xval={xvec}')
print(f'yval={yval}')
print(f'yder={yder}')
print(f'yint={yint}')

#ToContCmpd, FromContComp
anfreq = 1
inrate = 0.10
ccrate = orf.toContCmpd(inrate, anfreq)
outrate = orf.fromContCmpd(ccrate, anfreq)
print('To and from continuous compounding')
print(f'InRate={inrate}, CCRate={ccrate}, OutRate={outrate}')

#%%
# function group 1
print('=================')
print('Analytic prices')

#fwdprice
fwdpx = orf.fwdPrice(spot = 100, timetoexp = 1.0, intrate = 0.02, divyield = 0.04)
print(f'FwdPrice={fwdpx}')

##digibs
digi = orf.digiBS(payofftype = 1, spot = 100, timetoexp = 1.0, strike = 100,
                  intrate = 0.02, divyield = 0.04, volatility = 0.2)
print('Digital option using Black-Scholes analytic')
print(f'Price={digi}')

#eurobs
euro = orf.euroBS(payofftype = 1, spot = 100, timetoexp = 1.0, strike = 100,
                  intrate = 0.02, divyield = 0.04, volatility = 0.4)
print('European option using Black-Scholes analytic')
print(euro)

#%%
# function group 2
print('=================')
print('Market objects')

#yccreate
yc = orf.ycCreate(ycname = 'USD', 
                  tmats = [1/12, 1/4, 1/2, 3/4, 1, 2, 3, 4, 5], 
                  vals = [0.01, 0.012, 0.013, 0.014, 0.015, 0.02, 0.03, 0.04, 0.05],
                  valtype = 0)
print(f'Created yield curve: {yc}')

#discount, spotrate, fwdrate
df = orf.discount(ycname = yc, tmat = 2)
spotrate = orf.spotRate(ycname = yc, tmat = 2)
fwdrate = orf.fwdRate(ycname = yc, tmat1 = 1, tmat2 = 2)
print(f'DF={df}, SpotRate={spotrate} FwdRate={fwdrate}')

#volcreate
volts = orf.volCreate(volname = 'VOLTS', 
                      tmats = [1/4, 1/2, 3/4, 1, 1.5, 2, 3, 4], 
                      vals = [0.10, 0.12, 0.14, 0.16, 0.17, 0.18, 0.19, 0.20],
                      valtype = 0)
print(f'Created volatility term structure: {volts}')

#spotvol, fwdvol
spotvol = orf.spotVol(volname = volts, tmat = 2)
fwdvol = orf.fwdVol(volname = volts, tmat1 = 1, tmat2 = 2)
print(f'SpotVol={spotvol} FwdVol={fwdvol}')

print('Market list')
print(orf.mktList())

#%%
# function group 3
print('=================')
print('European option using Black-Scholes Monte Carlo')

#eurobsmc
mcpars0 = {'URNGTYPE': 'MT19937', 'PATHGENTYPE': 'EULER'}
npaths0 = 10000
euromc0 = orf.euroBSMC(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                       discountcrv =  yc, divyield = 0.04, volatility = 0.4,
                       mcparams = mcpars0, npaths = npaths0)
print(f'URNGTYPE={mcpars0["URNGTYPE"]} PATHGENTYPE={mcpars0["PATHGENTYPE"]} NPATHS={npaths0}')
print(f'Price={euromc0}')

mcpars1 = {'URNGTYPE': 'SOBOL', 'PATHGENTYPE': 'EULER'}
npaths1 = 10000
euromc1 = orf.euroBSMC(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                       discountcrv =  yc, divyield = 0.04, volatility = 0.4,
                       mcparams = mcpars1, npaths = npaths1)
print(f'URNGTYPE={mcpars1["URNGTYPE"]} PATHGENTYPE={mcpars1["PATHGENTYPE"]} NPATHS={npaths1}')
print(f'Price={euromc1}')

#%%
# function group 4
print('=================')
print('European option using Black-Scholes PDE')

#eurobspde
pdepars0 = {'NTIMESTEPS': 100, 'NSPOTNODES': 100, 'NSTDDEVS': 4, 'THETA': 1}

europde0 = orf.euroBSPDE(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                         discountcrv =  yc, divyield = 0.04, volatility = 0.4,
                         pdeparams = pdepars0)

print(f'NTIMESTEPS={pdepars0["NTIMESTEPS"]} NSPOTNODES={pdepars0["NSPOTNODES"]} NSTDDEVS={pdepars0["NSTDDEVS"]} THETA={pdepars0["THETA"]}')
print(f'Price={europde0["Price"]}')

pdepars1 = {'NTIMESTEPS': 100, 'NSPOTNODES': 100, 'NSTDDEVS': 4, 'THETA': 0.5}

europde1 = orf.euroBSPDE(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                         discountcrv =  yc, divyield = 0.04, volatility = 0.4,
                         pdeparams = pdepars1)
print(f'NTIMESTEPS={pdepars1["NTIMESTEPS"]} NSPOTNODES={pdepars1["NSPOTNODES"]} NSTDDEVS={pdepars1["NSTDDEVS"]} THETA={pdepars1["THETA"]}')
print(f'Price={europde1["Price"]}')

print('=================')
print('American option using Black-Scholes PDE')

amerpde0 = orf.amerBSPDE(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                         discountcrv =  yc, divyield = 0.04, volatility = 0.4,
                         pdeparams = pdepars0)
print(f'NTIMESTEPS={pdepars0["NTIMESTEPS"]} NSPOTNODES={pdepars0["NSPOTNODES"]} NSTDDEVS={pdepars0["NSTDDEVS"]} THETA={pdepars0["THETA"]}')
print(f'Price={amerpde0["Price"]}')

amerpde1 = orf.amerBSPDE(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                         discountcrv =  yc, divyield = 0.04, volatility = 0.4,
                         pdeparams = pdepars1)
print(f'NTIMESTEPS={pdepars1["NTIMESTEPS"]} NSPOTNODES={pdepars1["NSPOTNODES"]} NSTDDEVS={pdepars1["NSTDDEVS"]} THETA={pdepars1["THETA"]}')
print(f'Price={amerpde1["Price"]}')

#%%
# function group 5
print('=========================')
print('Portfolio return and risk')

assetrets = [0.10, 0.115, 0.08, 0.06]
assetvols = [0.20, 0.30, 0.22, 0.18]
correlmat = np.diag(4*[1.0])

mvpwts = orf.mvpWghts(assetrets, assetvols, correlmat)

mvprsk = orf.ptRisk(mvpwts, assetrets, assetvols, correlmat)

print('MVP_Weights =', mvpwts)

print('MVP_Return =', mvprsk['Mean'])
print('MVP_Risk =', mvprsk['StdDev'])
