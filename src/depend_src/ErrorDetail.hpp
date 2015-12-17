#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

///@brief：获取最新的错误原因
///@attention:  该类并不是一个安全的单例模式，如需安全，请用double-lock模式，也可以在程序初始化即初始化该对象
///参考资料：http://blog.chinaunix.net/uid-26611973-id-3078913.html

class CErrorDetail
{
public:
    CErrorDetail();
    ~CErrorDetail();

    static CErrorDetail* instance();

    inline const char* get_errmsg(int errorno);

private:
    inline void load_errmsg();

private:
    char m_errmsg[125][128];			//16000 byte
    static CErrorDetail* m_CErrorDetail;
};

CErrorDetail* CErrorDetail::m_CErrorDetail = NULL;

CErrorDetail::CErrorDetail()
{
}

CErrorDetail::~CErrorDetail()
{
    delete CErrorDetail::instance();
    CErrorDetail::m_CErrorDetail = NULL;
}

CErrorDetail* CErrorDetail::instance()
{
    if (m_CErrorDetail == NULL)
    {
        m_CErrorDetail = new(std::nothrow) CErrorDetail;
        if (m_CErrorDetail == NULL)
        {
            std::cout<<"m_CErrorDetail = new(std::nothrow) CErrorDetail failed."<<std::endl;
        }
        m_CErrorDetail->load_errmsg();
    }

    return m_CErrorDetail;
}
const char* CErrorDetail::get_errmsg(int errorno)
{
    char* pret = NULL;
    if (errorno <= 124)
    {
        pret = m_errmsg[errorno];
    }

    return pret;
}

void CErrorDetail::load_errmsg()
{
    memcpy(m_errmsg[0], "0 Success", 128);
    memcpy(m_errmsg[1], "1 EPERM:+Operation not permitted", 128);
    memcpy(m_errmsg[2], "2 ENOENT:+No such file or directory", 128);
    memcpy(m_errmsg[3], "3 ESRCH:+No such process", 128);
    memcpy(m_errmsg[4], "4 EINTR:+Interrupted system call", 128);
    memcpy(m_errmsg[5], "5 EIO:+Input/output error", 128);
    memcpy(m_errmsg[6], "6 ENXIO:+No such device or address", 128);
    memcpy(m_errmsg[7], "7 E2BIG:+Argument list too long", 128);
    memcpy(m_errmsg[8], "8 ENOEXEC:+Exec format error", 128);
    memcpy(m_errmsg[9], "9 EBADF:+Bad file descriptor", 128);
    memcpy(m_errmsg[10], "10 ECHILD:+No child processes", 128);
    memcpy(m_errmsg[11], "11 EAGAIN:+Resource temporarily unavailable", 128);
    memcpy(m_errmsg[12], "12 ENOMEM:+Cannot allocate memory", 128);
    memcpy(m_errmsg[13], "13 EACCES:+Permission denied", 128);
    memcpy(m_errmsg[14], "14 EFAULT:+Bad address", 128);
    memcpy(m_errmsg[15], "15 ENOTBLK:Block device required", 128);
    memcpy(m_errmsg[16], "16 EBUSY:+Device or resource busy", 128);
    memcpy(m_errmsg[17], "17 EEXIST:+File exists", 128);
    memcpy(m_errmsg[18], "18 EXDEV:+Invalid cross-device link", 128);
    memcpy(m_errmsg[19], "19 ENODEV:+No such device", 128);

    memcpy(m_errmsg[20], "20 ENOTDIR:+Not a directory", 128);
    memcpy(m_errmsg[21], "21 EISDIR:+Is a directory", 128);
    memcpy(m_errmsg[22], "22 EINVAL:+Invalid argument", 128);
    memcpy(m_errmsg[23], "23 ENFILE:+Too many open files in system", 128);
    memcpy(m_errmsg[24], "24 EMFILE:+Too many open files", 128);
    memcpy(m_errmsg[25], "25 ENOTTY:+Inappropriate ioctl for device", 128);
    memcpy(m_errmsg[26], "26 ETXTBSY:Text file busy", 128);
    memcpy(m_errmsg[27], "27 EFBIG:+File too large", 128);
    memcpy(m_errmsg[28], "28 ENOSPC:+No space left on device", 128);
    memcpy(m_errmsg[29], "29 ESPIPE:+No space left on device", 128);
    memcpy(m_errmsg[30], "30 EROFS:+Read-only file system", 128);
    memcpy(m_errmsg[31], "31 EMLINK:+Too many links", 128);
    memcpy(m_errmsg[32], "32 EPIPE:+Broken pipe", 128);
    memcpy(m_errmsg[33], "33 EDOM:+Numerical argument out of domain", 128);
    memcpy(m_errmsg[34], "34 ERANGE:+Numerical result out of range", 128);
    memcpy(m_errmsg[35], "35 EDEADLK:+Resource deadlock avoided", 128);
    memcpy(m_errmsg[36], "36 ENAMETOOLONG:+File name too long", 128);
    memcpy(m_errmsg[37], "37 ENOLCK:+No locks available", 128);
    memcpy(m_errmsg[38], "38 ENOSYS:+Function not implemented", 128);
    memcpy(m_errmsg[39], "39 ENOSYS:+Directory not empty", 128);

    memcpy(m_errmsg[40], "40 ELOOP:Too many levels of symbolic links", 128);
    memcpy(m_errmsg[41], "41 UNKNOW:NotUsed", 128);
    memcpy(m_errmsg[42], "42 ENOMSG:No message of desired type", 128);
    memcpy(m_errmsg[43], "43 EIDRM:Identifier removed", 128);
    memcpy(m_errmsg[44], "44 ECHRNG:Channel number out of range", 128);
    memcpy(m_errmsg[45], "45 EL2NSYNC:Level 2 not synchronized", 128);
    memcpy(m_errmsg[46], "46 EL3HLT:Level 3 halted", 128);
    memcpy(m_errmsg[47], "47 EL3RST:Level 3 reset", 128);
    memcpy(m_errmsg[48], "48 ELNRNG:Link number out of range", 128);
    memcpy(m_errmsg[49], "49 EUNATCH:Protocol driver not attached", 128);
    memcpy(m_errmsg[50], "50 ENOCSI:No CSI structure available", 128);
    memcpy(m_errmsg[51], "51 EL2HLT:Level 2 halted", 128);
    memcpy(m_errmsg[52], "52 EBADE:Invalid exchange", 128);
    memcpy(m_errmsg[53], "53 EBADR:Invalid request descriptor", 128);
    memcpy(m_errmsg[54], "54 EXFULL:Exchange full", 128);
    memcpy(m_errmsg[55], "55 ENOANO:No anode", 128);
    memcpy(m_errmsg[56], "56 EBADRQC:Invalid request code", 128);
    memcpy(m_errmsg[57], "57 EBADSLT:Invalid slot", 128);
    memcpy(m_errmsg[58], "58 UNKNOW:NotUsed", 128);
    memcpy(m_errmsg[59], "59 EBFONT:Bad font file format", 128);

    memcpy(m_errmsg[60], "60 ENOSTR:Device not a stream", 128);
    memcpy(m_errmsg[61], "61 ENODATA:No data available", 128);
    memcpy(m_errmsg[62], "62 ETIME:Timer expired", 128);
    memcpy(m_errmsg[63], "63 ENOSR:Out of streams resources", 128);
    memcpy(m_errmsg[64], "64 ENONET:Machine is not on the network", 128);
    memcpy(m_errmsg[65], "65 ENOPKG:Package not installed", 128);
    memcpy(m_errmsg[66], "66 EREMOTE:Object is remote", 128);
    memcpy(m_errmsg[67], "67 ENOLINK:Link has been severed", 128);
    memcpy(m_errmsg[68], "68 EADV:Advertise error", 128);
    memcpy(m_errmsg[69], "69 ESRMNT:Srmount error", 128);

    memcpy(m_errmsg[70], "70 ECOMM:Communication error on send", 128);
    memcpy(m_errmsg[71], "71 EPROTO:Protocol error", 128);
    memcpy(m_errmsg[72], "72 EMULTIHOP:Multihop attempted", 128);
    memcpy(m_errmsg[73], "73 EDOTDOT:RFS specific error", 128);
    memcpy(m_errmsg[74], "74 EBADMSG:+Bad message", 128);
    memcpy(m_errmsg[75], "75 EOVERFLOW:Value too large for defined data type", 128);
    memcpy(m_errmsg[76], "76 ENOTUNIQ:Name not unique on network", 128);
    memcpy(m_errmsg[77], "77 EBADFD:File descriptor in bad state", 128);
    memcpy(m_errmsg[78], "78 EREMCHG:Remote address changed", 128);
    memcpy(m_errmsg[79], "79 ELIBACC:Can not access a needed shared library", 128);

    memcpy(m_errmsg[80], "80 ELIBBAD:Accessing a corrupted shared library", 128);
    memcpy(m_errmsg[81], "81 ELIBSCN:.lib section in a.out corrupted", 128);
    memcpy(m_errmsg[82], "82 ELIBMAX:Attempting to link in too many shared libraries", 128);
    memcpy(m_errmsg[83], "83 ELIBEXEC:Cannot exec a shared library directly", 128);
    memcpy(m_errmsg[84], "84 EILSEQ:Invalid or incomplete multibyte or wide character", 128);
    memcpy(m_errmsg[85], "85 ERESTART:Interrupted system call should be restarted", 128);
    memcpy(m_errmsg[86], "86 ESTRPIPE:Streams pipe error", 128);
    memcpy(m_errmsg[87], "87 EUSERS:Too many users", 128);
    memcpy(m_errmsg[88], "88 ENOTSOCK:Socket operation on non-socket", 128);
    memcpy(m_errmsg[89], "89 EDESTADDRREQ:Destination address required", 128);

    memcpy(m_errmsg[90], "90 EMSGSIZE:+Message too long", 128);
    memcpy(m_errmsg[91], "91 EPROTOTYPE:Protocol wrong type for socket", 128);
    memcpy(m_errmsg[92], "92 ENOPROTOOPT:Protocol not available", 128);
    memcpy(m_errmsg[93], "93 EPROTONOSUPPORT:Protocol not supported", 128);
    memcpy(m_errmsg[94], "94 ESOCKTNOSUPPORT:Socket type not supported", 128);
    memcpy(m_errmsg[95], "95 EOPNOTSUPP:Operation not supported", 128);
    memcpy(m_errmsg[96], "96 EPFNOSUPPORT:Protocol family not supported", 128);
    memcpy(m_errmsg[97], "97 EAFNOSUPPORT:Address family not supported by protocol", 128);
    memcpy(m_errmsg[98], "98 EADDRINUSE:Address already in use", 128);
    memcpy(m_errmsg[99], "99 EADDRNOTAVAIL:Cannot assign requested address", 128);

    memcpy(m_errmsg[100], "100 ENETDOWN:Network is down", 128);
    memcpy(m_errmsg[101], "101 ENETUNREACH:Network is unreachable", 128);
    memcpy(m_errmsg[102], "102 ENETRESET:Network dropped connection on reset", 128);
    memcpy(m_errmsg[103], "103 ECONNABORTED:Software caused connection abort", 128);
    memcpy(m_errmsg[104], "104 ECONNRESET:Connection reset by peer", 128);
    memcpy(m_errmsg[105], "105 ENOBUFS:No buffer space available", 128);
    memcpy(m_errmsg[106], "106 EISCONN:Transport endpoint is already connected", 128);
    memcpy(m_errmsg[107], "107 ENOTCONN:Transport endpoint is not connected", 128);
    memcpy(m_errmsg[108], "108 ESHUTDOWN:Cannot send after transport endpoint shutdown", 128);
    memcpy(m_errmsg[109], "109 ETOOMANYREFS:Too many references: cannot splice", 128);

    memcpy(m_errmsg[110], "110 ETIMEDOUT:+Connection timed out", 128);
    memcpy(m_errmsg[111], "111 ECONNREFUSED:Connection refused", 128);
    memcpy(m_errmsg[112], "112 EHOSTDOWN:Host is down", 128);
    memcpy(m_errmsg[113], "113 EHOSTUNREACH:No route to host", 128);
    memcpy(m_errmsg[114], "114 EALREADY:Operation already in progress", 128);
    memcpy(m_errmsg[115], "115 EINPROGRESS:+Operation now in progress", 128);
    memcpy(m_errmsg[116], "116 ESTALE:Stale NFS file handle", 128);
    memcpy(m_errmsg[117], "117 EUCLEAN:Structure needs cleaning", 128);
    memcpy(m_errmsg[118], "118 ENOTNAM:Not a XENIX named type file", 128);
    memcpy(m_errmsg[119], "119 ENAVAIL:No XENIX semaphores available", 128);

    memcpy(m_errmsg[120], "120 EISNAM:Is a named type file", 128);
    memcpy(m_errmsg[121], "121 EREMOTEIO:Remote I/O error", 128);
    memcpy(m_errmsg[122], "122 EDQUOT:Disk quota exceeded", 128);
    memcpy(m_errmsg[123], "123 ENOMEDIUM:No medium found", 128);
    memcpy(m_errmsg[124], "124 EMEDIUMTYPE:Wrong medium type", 128);
}

