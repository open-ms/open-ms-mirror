///////////////////////////////////////////////////////////////////////////
//
//  PEAK DETECTION OF FOURIER TRANSFORME MS INSTRUMENT DATA
//
//  written by Markus Mueller, markus.mueller@imsb.biol.ethz.ch
//  ( and Lukas Mueller, Lukas.Mueller@imsb.biol.ethz.ch)
//  October 2005
//  
//  Ported to OpenMS by Florian Zeller, florian.zeller@bsse.ethz.ch
//  December 2010
//
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//

#ifndef USE_BACKGROUND_CONTROL_H
#define USE_BACKGROUND_CONTROL_H

class BackgroundControl {

private:
  
  map<double, map< double, BackgroundIntensityBin> > intensityBinMap;

  void init();

public:
  
  ~BackgroundControl();

  BackgroundControl();
  
  void addPeakMSScan( double , list<CentroidPeak>* peakList );
  
  double getBackgroundLevel( ms_peak* );
  
  double getBackgroundLevel( double mz, double tr);
  
  // find a key in the intensity map:
  map<double, map< double, BackgroundIntensityBin>  >::iterator findTrKey( double );
  
  // find a key in the m/z map:
  map< double, BackgroundIntensityBin>::iterator findMzKey( double mz, map< double, BackgroundIntensityBin>* );

  void processIntensityMaps(  );    
    
  // overload operators:
  bool operator==(const BackgroundControl&);
  BackgroundControl& operator<=(const BackgroundControl&);
  BackgroundControl& operator>=(const BackgroundControl&);
  BackgroundControl& operator<(const BackgroundControl&);
  BackgroundControl& operator>(const BackgroundControl&);
  
};

#endif

    
