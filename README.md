# appstream_parser

An `Appstream 1.0` XML parser written in C++.

Not yet spec compliant

### Introduction

Appstream XML 1.0 is used for describing Flatpak components.

A Flatpak component repository serves an `appstream.xml.gz` file that describes all the components it hosts.

One example of a Flatpak component registry is Flathub. The uncompressed `appstream.xml` that Flathub serves is quite
large:

```
-rw-rw-r-- 1 joel joel 36334733 Nov 11 09:22 appstream.xml
```

Working with large XML files presents challenges for limited resource computing devices. This application targets a
tradeoff between speed and runtime RAM footprint.

The goal of this application is the basis for creating a Flutter software catalog. Not all the Appstream data is
required to present to the user, this helps to reduce the RAM footprint; we only store the data we need to present.

#### DOM vs SAX parsers

Using a DOM parser the XML file is read at once, and a RAM representation of the XML document is created. This creates a
very large runtime footprint which is not conducive to constrained memory device.

SAX parsers read blocks at a time, and invoke user callbacks. This approach allows working with larger XML files with a
much lower memory footprint.

#### Memory Mapped files

The use of memory mapped files improves access time to the underlying file contents. The operating system handles paging
the file contents into RAM.

#### Minimizing Parser RAM usage

* Experiments show that libxml2 chunk reading generates the smallest RAM usage.
* Passing in the memory mapped file at once, ends up with a much larger RAM consumption.
* Parsing 1k chunks show the smallest RAM consumption.
* Increasing the read chunk size directly impacts RAM usage post parse. Which would indicate that the SAX parser cleans
  up heap allocations after each chunk parse.

#### Alternate XML libraries

* pugixml - (DOM parser) produces the largest RAM footprint. Not usable.
* rapidxml - (DOM parser) produces second largest RAM footprint. It has some nice features, and is high quality work. A
  SAX parser would be a nice addition. Note it does require C++20 minimum.

#### std::string_view usage

std::string_view can be used to wrap existing `non-null` strings.

std::string_view works with memory mapped files. With all the parsers tested none will reference the original string
pointer directly. Likely related to string fixups, and XML spec requirements. If the source data is known and no string
fixups are required, on could reference the raw pointers directly. Which would make parsing a large XML file much
faster and smaller.

Rapidxml leverages std::string_view, unfortunately tests show it produces 2x the RAM footprint. Likely this library
could be optimized to directly use pointers in the memory mapped file in order to limit heap allocations.

There could be more work in this area to improve RAM consumption.

This C library works around non-null strings in the XML by post-processing the XML data into a binary blob:
https://github.com/hughsie/libxmlb. This is a massive workaround to a problem better solved using std::string_view.

### References

* https://www.freedesktop.org/software/appstream/docs/
* https://github.com/flatpak/
* https://gitlab.gnome.org/GNOME/libxml2
* https://github.com/dwd/rapidxml
* https://github.com/zeux/pugixml
