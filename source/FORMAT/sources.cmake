### the directory name
set(directory source/FORMAT)

### list all filenames of the directory here
set(sources_list
Base64.C
Bzip2Ifstream.C
Bzip2InputStream.C
CompressedInputSource.C
CVMappingFile.C
ConsensusXMLFile.C
ControlledVocabulary.C
CsvFile.C
DTA2DFile.C
DTAFile.C
EDTAFile.C
FASTAFile.C
FastaIterator.C
FastaIteratorIntern.C
FeatureXMLFile.C
FileHandler.C
FileTypes.C
GzipIfstream.C
GzipInputStream.C
IdXMLFile.C
IndexedMzMLFile.C
InspectInfile.C
InspectOutfile.C
KroenikFile.C
LibSVMEncoder.C
MS2File.C
MSNumpressCoder.C
MSPFile.C
MascotInfile.C
MascotGenericFile.C
MascotRemoteQuery.C
MascotXMLFile.C
MsInspectFile.C
MzDataFile.C
MzTab.C
MzTabFile.C
MzMLFile.C
MzTabFile.C
MzXMLFile.C
OMSSACSVFile.C
OMSSAXMLFile.C
ParamXMLFile.C
PTMXMLFile.C
PeakTypeEstimator.C
PepNovoInfile.C
PepNovoOutfile.C
PepXMLFile.C
PepXMLFileMascot.C
ProtXMLFile.C
SequestInfile.C
SequestOutfile.C
SpecArrayFile.C
SVOutStream.C
TextFile.C
ToolDescriptionFile.C
TransformationXMLFile.C
UnimodXMLFile.C
XMassFile.C
XMLFile.C
XTandemInfile.C
XTandemXMLFile.C
MzIdentMLFile.C
MzQuantMLFile.C
QcMLFile.C
TraMLFile.C
)

### add path to the filenames
set(sources)
foreach(i ${sources_list})
	list(APPEND sources ${directory}/${i})
endforeach(i)

### pass source file list to the upper instance
set(OpenMS_sources ${OpenMS_sources} ${sources})

### source group definition
source_group("Source Files\\FORMAT" FILES ${sources})

