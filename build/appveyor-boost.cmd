set BOOST_VERSION=%1
if not exist C:\projects\deps\boost (
  set BOOST_URL="http://sourceforge.net/projects/boost/files/boost/%BOOST_VERSION%/boost_%BOOST_VERSION:.=_%.7z"
  appveyor DownloadFile %BOOST_URL% -FileName boost.7z
  7z x boost.7z -oC:\projects\deps > nul
  ren C:\projects\deps\boost_%BOOST_VERSION:.=_% boost
  del boost.7z
)
