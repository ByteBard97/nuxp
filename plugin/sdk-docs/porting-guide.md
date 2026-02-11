ADOBE® ILLUSTRATOR®




                   ADOBE
         ILLUSTRATOR 2026
           PORTING GUIDE
 2024 Adobe Incorporated. All rights reserved.

Adobe Illustrator 2026 Porting Guide

Technical Note #10500
If this guide is distributed with software that includes an end user agreement, this guide, as well as the software
described in it, is furnished under license and may be used or copied only in accordance with the terms of such license.
Except as permitted by any such license, no part of this guide may be reproduced, stored in a retrieval system, or
transmitted, in any form or by any means, electronic, mechanical, recording, or otherwise, without the prior written
permission of Adobe Incorporated. Please note that the content in this guide is protected under copyright law even if it
is not distributed with software that includes an end user license agreement.

The content of this guide is furnished for informational use only, is subject to change without notice, and should not be
construed as a commitment by Adobe Incorporated. Adobe Incorporated assumes no responsibility or liability for any
errors or inaccuracies that may appear in the informational content contained in this guide.

Please remember that existing artwork or images that you may want to include in your project may be protected under
copyright law. The unauthorized incorporation of such material into your new work could be a violation of the rights of
the copyright owner. Please be sure to obtain any permission required from the copyright owner.

Any references to company names in sample templates are for demonstration purposes only and are not intended to
refer to any actual organization.

Adobe, the Adobe logo, Creative Cloud, Illustrator, and Flash are either registered trademarks or trademarks of Adobe
Incorporated in the United States and/or other countries. Microsoft and Windows are either registered trademarks or
trademarks of Microsoft Corporation in the United States and/or other countries. Apple, Mac OS, and Macintosh are
trademarks of Apple Computer, Incorporated, registered in the United States and other countries. All other trademarks
are the property of their respective owners.

Adobe Incorporated, 345 Park Avenue, San Jose, California 95110, USA. Notice to U.S. Government End Users. The
Software and Documentation are “Commercial Items,” as that term is defined at 48 C.F.R. §2.101, consisting of
“Commercial Computer Software” and “Commercial Computer Software Documentation,” as such terms are used in 48
C.F.R. §12.212 or 48 C.F.R. §227.7202, as applicable. Consistent with 48 C.F.R. §12.212 or 48 C.F.R. §§227.7202-1 through
227.7202-4, as applicable, the Commercial Computer Software and Commercial Computer Software Documentation are
being licensed to U.S. Government end users (a) only as Commercial Items and (b) with only those rights as are granted
to all other end users pursuant to the terms and conditions herein. Unpublished-rights reserved under the copyright
laws of the United States. Adobe Incorporated, 345 Park Avenue, San Jose, CA 95110-2704, USA. For U.S. Government End
Users, Adobe agrees to comply with all applicable equal opportunity laws including, if appropriate, the provisions of
Executive Order 11246, as amended, Section 402 of the Vietnam Era Veterans Readjustment Assistance Act of 1974 (38
USC 4212), and Section 503 of the Rehabilitation Act of 1973, as amended, and the regulations at 41 CFR Parts 60-1
through 60-60, 60-250, and 60-741. The affirmative action clause and regulations contained in the preceding sentence
shall be incorporated by reference.
Adobe Illustrator 2026 Porting Guide


      This document describes how to update your SDK plug-in code and development environments for
      Adobe® Illustrator® 2026. It details changes in the public API and other aspects of the SDK since the
      previous release.

      Download the Illustrator 2026 SDK from https://console.adobe.io/downloads/ai, along with installation
      instructions and documentation.

      The Creative Cloud™ 2026 version requires a different development environment from previous releases;
      this means that you must recompile plug-ins built with an earlier version of the Illustrator SDK in order
      for them to run in Illustrator 2026. See “Development environment” on page 4.


SDK organization
      The SDK contains these folders and files (locations are relative to the download location, <SDK>):


      docs/                 Documentation, including:

                                Adobe Illustrator 2026Programmer’s Guide
                                 (guides/programmers-guide.pdf)

                                Porting Guide (this document)

                                Getting Started with Illustrator 2026 Development
                                 (guides/getting-started-guide.pdf)

                                Using the Adobe Text Engine (guides/using-adobe-text-engine.pdf)

                                API Reference

                                 This document is provided in two formats:

                                     references/index.chm — This compiled HTML file allows text searches

                                 to be performed on the content. See the Getting Started with Illustrator 2026
                                 Development for details.

                                     references/sdkdocs.tar.gz — This file contains the API Reference in
                                      HTML format. To view the contents, decompress the archive, then open
                                      index.html in your browser.

                                API Advisor (references/apiadvisor-ai18-vs-ai19.html)

      legalnotices/         Licenses.
      illustratorapi        The Illustrator API header files.
      samplecode/           A set of samples for learning about the API.




                                                                                                                 3
Adobe Illustrator 2026 Porting Guide                                                              Changes in this release   4



Changes in this release
                These are the major changes between this release and the previous release.


      Development environment
                The version of Visual Studio has changed. When you open an older solution or project file in the new
                version of Visual Studio, you are prompted to update the file.

                This release supports these platforms for Illustrator plug-in development.


                Platform               Component                 Note
                Windows                Windows 10
                                       Illustrator 2026

                                       Visual Studio 2022
                     Visual Studio 2010 replaced the _SECURE_SCL and _HAS_ITERATOR_DEBUGGING flags with an
                     _ITERATOR_DEBUG_LEVEL macro. Illustrator defines _ITERATOR_DEBUG_LEVEL=0 for Release, and
                     _ITERATOR_DEBUG_LEVEL=2 for Debug. We recommend that developers do the same in Visual
                     Studio 2022.


                Mac OS®                macOS 12.0 or higher      Deployment Target should be macOS
                                                                 12.0
                                       SDK: macosx (default)
                                       Xcode 15.2                Xcode can be downloaded from
                                                                 https://developer.apple.com/download/more/
                                       LLVM Clang

                 Language         Standard


                  C++             17


      Illustrator API changes
                A detailed change list is provided with the SDK, in the API Advisor page, apiadvisor-report.html.
