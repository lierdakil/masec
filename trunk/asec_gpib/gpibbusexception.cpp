#include "gpibbusexception.h"

GPIBBusException::GPIBBusException(int err)
{
    //setMessage(QString::fromUtf8("There was an error #%1 with message '%2'").arg(status).arg(QString::fromLocal8Bit(desc)));
    switch(err)
    {
    case 0:	setMessage("EDVR: A system call has failed. ibcnt/ibcntl will be set to the value of errno.");
        break;
    case 1:	setMessage("ECIC: Your interface board needs to be controller-in-charge, but is not.");
        break;
    case 2:	setMessage("ENOL: You have attempted to write data or command bytes, but there are no listeners currently addressed.");
        break;
    case 3:	setMessage("EADR: The interface board has failed to address itself properly before starting an io operation.");
        break;
    case 4:	setMessage("EARG: One or more arguments to the function call were invalid.");
        break;
    case 5:	setMessage("ESAC: The interface board needs to be system controller, but is not.");
        break;
    case 6:	setMessage("EABO: A read or write of data bytes has been aborted, possibly due to a timeout or reception of a device clear command.");
        break;
    case 7:	setMessage("ENEB: The GPIB interface board does not exist, its driver is not loaded, or it is not configured properly.");
        break;
    case 10:	setMessage("EOIP: Function call can not proceed due to an asynchronous IO operation (ibrda(), ibwrta(), or ibcmda()) in progress.");
        break;
    case 11:	setMessage("ECAP: Incapable of executing function call, due the GPIB board lacking the capability, or the capability being disabled in software.");
        break;
    case 12:	setMessage("EFSO: File system error. ibcnt/ibcntl will be set to the value of errno.");
        break;
    case 14:	setMessage("EBUS: An attempt to write command bytes to the bus has timed out.");
        break;
    case 15:	setMessage("ESTB: One or more serial poll status bytes have been lost. This can occur due to too many status bytes accumulating (through automatic serial polling) without being read.");
        break;
    case 16:	setMessage("EQRQ: The serial poll request service line is stuck on. This can occur if a physical device on the bus requests service, but its GPIB address has not been opened (via ibdev() for example) by any process. Thus the automatic serial polling routines are unaware of the device's existence and will never serial poll it.");
        break;
    case 20:	setMessage("ETAB: can be returned by ibevent(), FindLstn(), or FindRQS(). See their descriptions for more information.");
        break;
    }
}
