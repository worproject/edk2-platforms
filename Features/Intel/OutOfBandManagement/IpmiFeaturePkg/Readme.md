# Overview
* **Feature Name:** Intelligent Platform Management Interface (IPMI)
* **PI Phase(s) Supported:** PEI, DXE, SMM
* **SMM Required?** Yes

More Information:
* [IPMI Specification 2nd Generation v2.0](https://www.intel.com/content/dam/www/public/us/en/documents/product-briefs/ipmi-second-gen-interface-spec-v2-rev1-1.pdf)

## Purpose
The IPMI feature provides firmware functionality that implements behavior described in the IPMI specification. IPMI
enables out-of-band and monitoring capabilities independent of the host system's CPU, firmware, and operating system.

# High-Level Theory of Operation
*_TODO_*
A description of how the device works at a high-level.

The description should not be constrained to implementation details but provide a simple mental model of how the
feature is supposed to work.

## Firmware Volumes
*_TODO_*
A bulleted list of the firmware volumes that feature module(s) are placed in.

## Modules
*_TODO_*
A bulleted list of the modules that make up the feature.

## <Module Name>
*_TODO_*
Each module in the feature should have a section that describes the module in a level of detail that is useful
to better understand the module source code.

## <Library Name>
*_TODO_*
Each library in the feature should have a section that describes the library in a level of detail that is useful
to better understand the library source code.

#BmcInterfaceCommonAccess
This Library provides the common API's functions among the different Interfaces such as BT, SSIF,and IPMB interface.

#BtInterfaceLib
This Library provides the API's function for the BT interface.

#IpmbInterfaceLib
This Library provides the API's function for the IPMB/I2C interface.

#SsifInterfaceLib
This Function provides the API's function for the SSIF interface.

## Key Functions
*_TODO_*
A bulleted list of key functions for interacting with the feature.

Not all features need to be listed. Only functions exposed through external interfaces that are important for feature
users to be aware of.

* Some BMC may have various interface support, in this case IpmiTransport2Protocol will check the available interfaces Supports
and initialize the Interface API for IPMI communication. it will check for multiple interfaces  such as KCS,BT,SSIF and IPMB/I2C interface.
* PcdDefaultSystemInterface, helps to select the default Interface type to communicate with BMC.
IpmiSubmitCommand2 API will send the IPMI command via by selected PcdDefaultSystemInterface Type.
* We can able to send the IPMI command via the specific Interface type through IpmiSubmitCommand2Ex.
* In case if the platform BMC doesn't have any interface support, we can disable the
respective Interface support(PcdKcsInterfaceSupport,PcdBtInterfaceSupport,PcdSsifInterfaceSupport and PcdIpmbInterfaceSupport)

## Configuration
*_TODO_*
Information that is useful for configuring the feature.

Not all configuration options need to be listed. This section is used to provide more background on configuration
options than possible elsewhere.

* While selecting Default Interface type on PcdDefaultSystemInterface, the respective Interface Support should need to be Enabled,
else the IpmiTransport2Protocol will be unsupported. Example, for SysInterfaceKcs on PcdDefaultSystemInterface, PcdKcsInterfaceSupport Should be enable.


## Data Flows
*_TODO_*
Architecturally defined data structures and flows for the feature.

## Control Flows
*_TODO_*
Key control flows for the feature.

## Build Flows
Supported build targets
* VS2019
* CLANGPDB
* GCC5

## Test Point Results
*_TODO_*
The test(s) that can verify porting is complete for the feature.

Each feature must describe at least one test point to verify the feature is successful. If the test point is not
implemented, this should be stated.

## Functional Exit Criteria
*_TODO_*
The testable functionality for the feature.

This section should provide an ordered list of criteria that a board integrator can reference to ensure the feature is
functional on their board.

## Feature Enabling Checklist
*_TODO_*
An ordered list of required activities to achieve desired functionality for the feature.

## Performance Impact
A general expectation for the impact on overall boot performance due to using this feature.

This section is expected to provide guidance on:
* How to estimate performance impact due to the feature
* How to measure performance impact of the feature
* How to manage performance impact of the feature

## Common Optimizations
*_TODO_*
Common size or performance tuning options for this feature.

This section is recommended but not required. If not used, the contents should be left empty.
