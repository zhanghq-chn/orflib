import orflib.pyorflib

###################
# function group 0

def version(): 
    """orflib library version.

    Returns
    -------
    str
        version in the format Major.Minor.Revision
    """
    return pyorflib.version()

def matrixEcho(mat):
    """Echoes the input matrix.

    Parameters
    ----------
    mat : 2D numpy array
        input matrix

    Returns
    -------
    2D numpy array
        the input matrix `mat`
    """
    return pyorflib.matrixEcho(mat)

def cholDcmp(mat): 
    """Cholesky decomposition of the input matrix. 

    Parameters
    ----------
    mat : 2D numpy array
        input square matrix

    Returns
    -------
    2D numpy array
       lower triangular Cholesky factor
    """
    return pyorflib.cholDcmp(mat)

def eigenSym(mat): 
    """Eigenvalues and eigenvectors of a symmetric matrix. 

    Parameters
    ----------
    mat : 2D numpy array
        input symmetric matrix

    Returns
    -------
    dictionary
        Eigenvals :  1D numpy array
            the eigenvalues
        Eigenvecs : 2D numpy array
            the eigenvectors in column order
    """
    return pyorflib.eigenSym(mat)

def specTrunc(mat): 
    """Nearest positive definite matrix using spectral truncation. 

    Parameters
    ----------
    mat : 2D numpy array
        input symmetric matrix

    Returns
    -------
    2D numpy array
        the nearest to `mat` positive definite matrix
    """
    return pyorflib.specTrunc(mat)

def ppolyEval(bkpoints, values, polyorder, xvec, derivorder): 
    """Values and derivatives of a piecewise polynomial function at a set of points.

    Parameters
    ----------
    bkpoints : list(double) or 1D numpy array
        breakpoints of the piecewise polynomial function
    values : list(double) or 1D numpy array
        corresponding values at each breakpoint
    polyorder : {0, 1}
        polynomial order of the segments (constant or linear)
    xvec : list(double) or 1D numpy array
        points at which values or derivatives are evaluated
    derivorder : int (>= 0)
        if 0, values are computed, else derivatives of `derivorder` order  

    Returns
    -------
    1D numpy array
        values or derivatives at each point in `xvec`
    
    Notes
    -----
    1. If `polyorder`==0, the value at the left breakpoint is used for the segment.
    2. At points outside the breakpoint span constant extrapolation is used.
    """
    return pyorflib.ppolyEval(bkpoints, values, polyorder, xvec, derivorder)

def ppolyIntegral(bkpoints, values, polyorder, xstart, xvec): 
    """Integral of a piecewise polynomial function from `xstart` to each point in `xvec`.

    Parameters
    ----------
    bkpoints : list(double) or 1D numpy array
        brekpoints of the piecewise polynomial function
    values : list(double) or 1D numpy array
        corresponding values at each breakpoint
    polyorder : {0, 1}
        polynomial order of the segments (constant or linear)
    xstart : double
        starting point of each interval
    xvec : list(double) or 1D numpy array
        end points one for each interval

    Returns
    -------
    1D numpy array
        values of the integral
    
    Notes
    -----
    1. If `polyorder`==0, the value at the left breakpoint is used for the segment.
    2. At points outside the breakpoint span constant extrapolation is used.
    """
    return pyorflib.ppolyIntegral(bkpoints, values, polyorder, xstart, xvec)

def toContCmpd(rate, annfreq): 
    """Continuously compounded from periodically compounded interest rate. 

    Parameters
    ----------
    rate : double
        periodically compounded rate
    annfreq : int (> 0)
        the annual compounding frequency

    Returns
    -------
    double
        equivalent continuously compounded rate
    """
    return pyorflib.toContCmpd(rate, annfreq)

def fromContCmpd(rate, annfreq): 
    """Periodically compounded from continuously compounded interest rate. 

    Parameters
    ----------
    rate : double
        continuously compounded rate
    annfreq : int (> 0)
        the annual comounding frequency

    Returns
    -------
    double
        equivalent periodically compounded rate
    """
    return pyorflib.fromContCmpd(rate, annfreq)

###################
# function group 1

def erf(x):
    """Error function at `x`. 
    """
    return pyorflib.erf(x)

def invErf(x):
    """Inverse error function at `x`. 
    """
    return pyorflib.invErf(x)

def normalCdf(x):
    """Cumulative standard normal distribution at `x`. 
    """
    return pyorflib.normalCdf(x)

def normalInvCdf(x):
    """Inverse of the cumulative standard normal distribution at quantile `x`.
    """
    return pyorflib.normalInvCdf(x)

def fwdPrice(spot, timetoexp, intrate, divyield):
    """Forward price of an asset.

    Parameters
    ----------
    spot : double
        asset spot price
    timetoexp : double
        time to expiration in years
    intrate : double
        interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.

    Returns
    -------
    double
        asset forward price
     """
    return pyorflib.fwdPrice(spot, timetoexp, intrate, divyield)


def digiBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility):
    """Price of a European digital option in the Black-Scholes model. 

    Parameters
    ----------
    payofftype : {1, -1}
        1 for call, -1 for put
    spot : double
        asset spot price
    strike : double
        strike price
    timetoexp : double
        time to expiration in years
    intrate : double
        interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.
    volatility : double
        asset return volatility

    Returns
    -------
    dictionary
        Price : price of the option
        Delta : Delta of the option
        Gamma : Gamma of the option    """
    return pyorflib.digiBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility)

def euroBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility):
    """Price and Greeks of a European option in the Black-Scholes model. 

    Parameters
    ----------
    payofftype : {1, -1}
        1 for call, -1 for put
    spot : double
        asset spot price
    strike : double
        strike price
    timetoexp : double
        time to expiration in years
    intrate : double
        interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.
    volatility : double
        asset return volatility

    Returns
    -------
    dictionary
        Price : price of the option
        Delta : Delta of the option
        Gamma : Gamma of the option
    """
    return pyorflib.euroBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility)

def sqeuroBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility):
    """Price of a Squared European option in the Black-Scholes model. 

    Parameters
    ----------
    payofftype : {1, -1}
        1 for call, -1 for put
    spot : double
        asset spot price
    strike : double
        strike price
    timetoexp : double
        time to expiration in years
    intrate : double
        interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.
    volatility : double
        asset return volatility

    Returns
    -------
     double
        price of option
    """
    return pyorflib.sqeuroBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility)

def kogwd(spot, strike, kolevel, timetoexp, timetoko, intrate, divyield, volatility):
    """Price of a Squared European option in the Black-Scholes model. 

    Parameters
    ----------
    spot : double
        asset spot price
    strike : double
        strike price
    kolevel : double
        knock-out level
    timetoexp : double
        time to expiration in years
    timetoko : double
        time to knock-out in years
    intrate : double
        interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.
    volatility : double
        asset return volatility

    Returns
    -------
     double
        price of knock-out forward contract
    """
    return pyorflib.kofwd(spot, strike, kolevel, timetoexp, timetoko, intrate, divyield, volatility)
    
###################
# function group 2

def mktList():
    """Names of all market objects.

    Returns
    -------
    dictionary
        YieldCurves : list with names of yield curves
        Volatilities : list with names of volatility term structures   
    """
    return pyorflib.mktList()

def mktClear():
    """Clears all market objects.

    Returns
    -------
    TRUE
    """
    return pyorflib.mktClear()

def ycCreate(ycname, tmats, vals, valtype):
    """Creates a new yield curve.

    Parameters
    ----------
    ycname : str
        new yield curve name
    tmats : list(double) or 1D numpy array
        maturities in years
    vals : list(double) or 1D numpy array
        zero bond prices or interest rates to each maturity in `tmats`
    valtype : {0, 1, 2}
        0: zero bond prices, 1: spot interest rates, 2: forward interest rates

    Returns
    -------
    str 
        name of the newly created yield curve

    Notes
    -----
    1. Yield curve names are case insensitive.
    2. A new yield curve replaces an existing one if they have the same name.
    """
    return pyorflib.ycCreate(ycname, tmats, vals, valtype)

def discount(ycname, tmat):
    """Discount factor from yield curve.

    Parameters
    ----------
    ycname : str
        name of the yield curve
    tmat : double
        time to maturity of the discount factor, in years

    Returns
    -------
    double
        discount factor
    """
    return pyorflib.discount(ycname, tmat)

def fwdDiscount(ycname, tmat1, tmat2):
    """Forward discount factor from yield curve.

    Parameters
    ----------
    ycname : str
        name of the yield curve
    tmat1 : double
        time to reset of the forward discount factor, in years
    tmat2 : double
        time to maturity of the forward discount factor, in years

    Returns
    -------
    double
        forward discount factor
    """
    return pyorflib.fwdDiscount(ycname, tmat1, tmat2)

def spotRate(ycname, tmat):
    """Spot interest rate from yield curve.

    Parameters
    ----------
    ycname : str
        name of the yield curve
    tmat : double
        time to maturity of the interest rate, in years

    Returns
    -------
    double
        spot interest rate
    """
    return pyorflib.spotRate(ycname, tmat)

def fwdRate(ycname, tmat1, tmat2):
    """Forward interest rate from yield curve.

    Parameters
    ----------
    ycname : str
        name of the yield curve
    tmat1 : double
        time to reset of the forward discount rate, in years
    tmat2 : double
        time to maturity of the forward discount rate, in years

    Returns
    -------
    double
        the forward discount rate
    """
    return pyorflib.fwdRate(ycname, tmat1, tmat2)

def volCreate(volname, tmats, vals, valtype):
    """Creates a new volatility term structure.

    Parameters
    ----------
    volname : str
        name of the volatility term structure
    tmats : list(double) or 1D numpy array
        maturities (expirations) in years
    vals : list(double) or 1D numpy array
        volatility values to each maturity in `tmats`
    valtype : {0, 1}
        0: spot volatilities, 1: forward volatilities

    Returns
    -------
    str 
        name of the newly created volatility term structure
    """
    return pyorflib.volCreate(volname, tmats, vals, valtype)

def spotVol(volname, tmat):
    """Spot volatility from term structure.

    Parameters
    ----------
    volname : str
        name of the volatility term structure
    tmat : double
        time to maturity (expiration) of the spot volatility, in years

    Returns
    -------
    double
        spot volatility
    """
    return pyorflib.spotVol(volname, tmat)

def fwdVol(volname, tmat1, tmat2):
    """Forward volatility from term structure.

    Parameters
    ----------
    volname : str
        name of the volatility term structure
    tmat1 : double
        time to reset of the forward volatility, in years
    tmat2 : double
        time to maturity (expiration) of the forward volatility, in years

    Returns
    -------
    double
        forward volatility
    """
    return pyorflib.fwdVol(volname, tmat1, tmat2)

def capFloorletBS(payofftype, ycname, strikerate, timetoreset, tenor, fwdratevol):
    """Present value of a caplet or floorlet on a fowrward rate in the Black-Scholes model.

    Parameters
    ----------
    payofftype : {1, -1}
        1: caplet, -1: floorlet
    ycname : str
        yield curve name
    strikerate : double
        strike interest rate
    timetoreset : double
        time to caplet reset in years
    tenor : double
        tenor duration in years
    fwdratevol : double
        volatility of the forward rate resetting at `timetoreset` 
        and expiring at `timetoreset` + `tenor`

    Returns
    -------
    double
        price of the caplet or floorlet
    """
    return pyorflib.capFloorletBS(payofftype, ycname, strikerate, timetoreset, tenor, fwdratevol)

def cdsPV(ycname, credspread, cdsrate, recov, timetomat, payfreq):
    """Present value of the default and premium leg of a CDS.

    Parameters
    ----------
    ycname : str
        default-free yield curve name
    credspread : double
        credit spread, c.c. and p.a.
    cdsrate : double
        CDS premium rate 
    recov : double
        recovery rate in case of default
    timetomat : double
        time to maturity of the CDS, in years
    payfreq : int
        payment frequency, i.e. number of premium payments per year

    Returns
    -------
    list(double)
        PV of the default and premium leg of the CDS
    """
    return pyorflib.cdsPV(ycname, credspread, cdsrate, recov, timetomat, payfreq)

###################
# function group 3

def euroBSMC(payofftype, strike, timetoexp, spot, discountcrv, divyield, volatility, mcparams, npaths):
    """Price and standard error of a European option in the Black-Scholes model using Monte Carlo.

    Parameters
    ----------
    payofftype : {1, -1}
        1 for call, -1 for put
    strike : double
        strike price
    timetoexp : double
        time to expiration in years
    spot : double
        asset spot price
    discountcrv : str
        discount yield curve name
    divyield : double    
        asset dividend yield, p.a. and c.c.
    volatility : double
        asset return volatility
    mcparams : dictionary
        URNGTYPE : 'MINSTDRAND', 'MT19937', 'RANLUX3', 'RANLUX4', 'SOBOL'
        PATHGENTYPE : 'EULER'
        CONTROLVARTYPE : 'ANTITHETIC', 'NONE'
    npaths : int
        number of Monte Carlo paths
    
    Returns
    -------
    dictionary
        Mean : Monte Carlo mean price
        StdErr : Monte Carlo standard error
    """
    return pyorflib.euroBSMC(payofftype, strike, timetoexp, spot, discountcrv, divyield, volatility, mcparams, npaths)

def sobolSeq(npoints, ndims, distrib):
    """Sobol points in `ndims` dimensions
    
    Parameters
    ----------
    npoints : int
        number of points to generate
    ndims : int
        number of dimensions of space in which the points live
    
    distrib : {0, 1}
        0: uniform. 1: standard normal distribution

    Returns
    -------
    2D numpy array
        matrix with shape (`npoints`, `ndims`) with the Sobol points, one point per row
    """
    return pyorflib.sobolSeq(npoints, ndims, distrib)

###################
# function group 4

def euroBSPDE(payofftype, strike, timetoexp, spot, discountcrv, divyield, volatility, pdeparams, allresults=False):
    """Price of a European option in the Black-Scholes model using finite difference PDE.

    Parameters
    ----------
    payofftype : {1, -1}
        1 for call, -1 for put
    strike : double
        strike price
    timetoexp : double
        time to expiration in years
    spot : double
        asset spot price
    discountcrv : str
        discount yield curve name
    divyield : double    
        asset dividend yield, p.a. and c.c.
    volatility : double
        asset return volatility
    pdeparams : dictionary
        NTIMESTEPS : (int) number of time steps
        NSPOTNODES : (int) number of spot nodes
        NSTDDEVS : (double) number of standard deviations for the spot range
        THETA : (double) scheme implicitness
    allresults : bool
        FALSE for price only; TRUE for the full grid of results
    
    Returns
    -------
    dictionary
        Price : PDE price
        Times : 1D array with times
        Spots : 1D array with spots
        Values : 2D array with option values

    Notes
    -----
    The keys `Times`, `Spots` and `Values` are available only if `allresults`==True.
    """
    return pyorflib.euroBSPDE(payofftype, strike, timetoexp, spot, discountcrv, divyield, volatility, pdeparams, allresults)

def amerBSPDE(payofftype, strike, timetoexp, spot, discountcrv, divyield, volatility, pdeparams, allresults=False):
    """Price of an American option in the Black-Scholes model using finite difference PDE.

    Parameters
    ----------
    payofftype : {1, -1}
        1 for call, -1 for put
    strike : double
        strike price
    timetoexp : double
        time to expiration in years
    spot : double
        asset spot price
    discountcrv : str
        discount yield curve name
    divyield : double    
        asset dividend yield, p.a. and c.c.
    volatility : double
        asset return volatility
    pdeparams : dictionary
        NTIMESTEPS : (int) number of time steps
        NSPOTNODES : (int) number of spot nodes
        NSTDDEVS : (double) number of standard deviations for the spot range
        THETA : (double) scheme implicitness
    allresults : bool
        FALSE for price only; TRUE for the full grid of results
    
    Returns
    -------
    dictionary
        Price : PDE price
        Times : 1D array with times
        Spots : 1D array with spots
        Values : 2D array with option values

    Notes
    -----
    The keys `Times`, `Spots` and `Values` are available only if `allresults`==True.
    """
    return pyorflib.amerBSPDE(payofftype, strike, timetoexp, spot, discountcrv, divyield, volatility, pdeparams, allresults)

###################
# function group 5

def ptRisk(ptwghts, assetrets, assetvols, correlmat):
    """Mean and standard deviation of portfolio return.

    Parameters
    ----------
    ptwghts : list(double) or 1D numpy array
        portfolio weights for each contained asset
    assetrets :  list(double) or 1D numpy array
        expected asset returns
    assetvols : list(double) or 1D numpy array
        asset return volatilities
    correlmat : 2D numpy array
        asset return correlation matrix

    Returns
    -------
    dictionary
        Mean : mean porfolio return
        StdDev : standard deviation of portfolio return
    """
    return pyorflib.ptRisk(ptwghts, assetrets, assetvols, correlmat)

def mvpWghts(assetrets, assetvols, correlmat):
    """Weights of minimum variance portfolio.

    Parameters
    ----------
    assetrets :  list(double) or 1D numpy array
        expected asset returns
    assetvols : list(double) or 1D numpy array
        asset return volatilities
    correlmat : 2D numpy array
        asset return correlation matrix

    Returns
    -------
    1D numpy array
        the minimum variance portfolio weights
    """
    return pyorflib.mvpWghts(assetrets, assetvols, correlmat)
