AppVeyor does not have the capibility to understand CMake's testing framework, ctest's, XML output.
This PowerShell script enables AppVeyor to parse these XMLs.

In order to obtain such an XML, add `-T Test` as a parameter to `ctest`.

To upload the results to the CI system use something like this snippet:

```
foreach($file in $(ls Testing\*\Test.xml))
{
    upload-ctest-report $file
}
```

This is essently a workaround until AppVeyor supports CMake's XML format natively.
It transforms the XML into the JUnit format, what AppVeyor can understand using [zanata's](https://raw.githubusercontent.com/zanata/zanata-tests/master/scripts/CTest2JUnit.xsl) XSLT.
