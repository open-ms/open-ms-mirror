// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2013.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Florian Zeller $
// $Authors: Lukas Mueller, Markus Mueller $
// --------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//  Ported to OpenMS by Florian Zeller, florian.zeller@bsse.ethz.ch
//  December 2010
//

#ifndef OPENMS_TRANSFORMATIONS_FEATUREFINDER_SUPERHIRN_SHFEATURE_H
#define OPENMS_TRANSFORMATIONS_FEATUREFINDER_SUPERHIRN_SHFEATURE_H

namespace OpenMS
{

  class OPENMS_DLLAPI SHFeature
  {

    ////////////////////////////////////////////////
    // declaration of the private members:

private:

    /////////////////////////////////////////////
    // IDENTIFICATION PARAMETERS:
    // name of the spectra:
    std::map<double, std::vector<MS2Info> > MS2_SCANS;
    /////////////////////////////////////////////

    /////////////////////////////////////////////
    // raw MS peak parameters:
    int scan_apex;
    int scan_start;
    int scan_end;
    double total_peak_area;
    double apex_peak_intensity;
    double PEAK_SCORE;
    double SignalToNoise;
    double BackgroundNoise;

    ////////////////////////////////////////////
    // Analysis parameters:
    double alignment_error_up;
    double alignment_error_down;
    double SCORE_HOLDER;
    bool feature_match_status;
    double PI;

    ////////////////////////////////////////////
    // LC/MS run ID parameters:
    int spectrum_ID;
    int MASTER_ID;

    ///////////////////////////////////////////
    // string to store ms1 feature extra information:
    std::string featureExtraInformation;

    ///////////////////////////////////////////
    // LC elution profile:
    FeatureLCProfile * LCprofile;

    // static values:
    static double _MONO_H;
    static double _MONO_O;

    //////////////////////////////////////////
    // LC/MS matching things:
    std::map<int, SHFeature> matched_feature_list;

    // ranges of m/z and tr:
    double TR_APEX;
    double MONO_MZ_START;
    double MONO_MZ_END;
    double MONO_MZ_ORIGINAL;

    //////////////////////////////////////////
    // associated MS2 feature:
    MS2Feature * MS2TraceFeature;

    ////////////////////////////////////////////////
    // declaration of the public members:

public:

    double TR;
    double MONO_MZ;
    double TR_START;
    double TR_END;
    int charge_state;
    int feature_ID;

    // class destructor
    ~SHFeature();

    // copy constructor:
    SHFeature(const SHFeature &);

    // class constructor
    SHFeature(const SHFeature *);
    // constructor for the object SHFeature:
    SHFeature(double, double, int, int, int, int, float, float, float);
    // constructor for the object SHFeature:
    SHFeature(float, int, int);
    SHFeature(MS2Feature *);
    SHFeature();
    // copy constructor:
    SHFeature & operator=(const SHFeature &);

    // show the content of the spectra
    void show_info();
    // show MS/MS spectra info:
    void showMS2consensSpectraInfo();

    //////////////////////////////////
    // comparison operators:
    bool operator==(const SHFeature &);

    // add MS/MS info to the SHFeature:
    void add_MS2_info(MS2Info *);
    void add_MS2_info(std::map<double, std::vector<MS2Info> > *);
    bool get_MS2_info();
    bool get_MS2_info(double);

    bool check_MS2_empty()
    {
      return MS2_SCANS.empty();
    }

    void removeAllMS2Information()
    {
      return MS2_SCANS.clear();
    }

    int get_MS2_SCANS_SIZE()
    {
      return (int) MS2_SCANS.size();
    }

    std::map<double, std::vector<MS2Info> > * get_MS2_SCAN_MAP()
    {
      return &MS2_SCANS;
    }

    std::map<double, std::vector<MS2Info> >::iterator get_MS2_SCANS_START()
    {   return MS2_SCANS.begin(); }
    std::map<double, std::vector<MS2Info> >::iterator get_MS2_SCANS_END()
    {   return MS2_SCANS.end(); }
    // get the best ms2 scan == closest to the apex:
    MS2Info * get_best_MS2_SCAN();
    MS2Info * get_best_MS2_SCAN(double);

    void setFeatureExtraInformation(std::string in)
    {   featureExtraInformation = in; }
    std::string getFeatureExtraInformation()
    {   return featureExtraInformation; }

    // functions to set/access matched features:
    void add_matched_feature(SHFeature *);
    std::map<int, SHFeature> * get_match_list_REFERENCE()
    {   return &matched_feature_list; }
    std::map<int, SHFeature> get_match_list()
    {   return matched_feature_list; }
    std::map<int, SHFeature>::iterator get_match_list_start()
    {   return matched_feature_list.begin(); }
    std::map<int, SHFeature>::iterator get_match_list_end()
    {   return matched_feature_list.end(); }
    std::map<int, SHFeature>::iterator find_match_by_id(int ID)
    {   return matched_feature_list.find(ID); }

    // get feature at a certain LC-MS by LC_MS id
    SHFeature * get_feature(int);

    // get the total peak are over all matched features:
    double get_MATCHED_peak_area();
    bool check_match_by_id(int);
    void erase_match_list()
    {   matched_feature_list.clear(); }
    // get the profile over all matched features:
    std::map<int, double> get_feature_profile();

    // return number of times this feature has been seen = nb_replicates in list plus 1!
    int get_replicate_match_nb()
    {   return (int) (matched_feature_list.size() + 1); }
    int get_matching_nb()
    {   return get_replicate_match_nb(); }
    // return the sum of all intensities over replicates:
    double get_replicate_intensity_sum();

    ///////////////////////////////
    // start here all the get / set
    // function to access the
    // variables of the class

    // access the parent mass of feature, calculated from the SQ
    double get_MZ()
    {   return MONO_MZ; }
    void set_MZ(double in)
    {   MONO_MZ = in; }
    double get_MZ_START()
    {   return MONO_MZ_START; }
    void set_MZ_START(double IN)
    {   MONO_MZ_START = IN; }
    double get_MZ_END()
    {   return MONO_MZ_END; }
    void set_MZ_END(double IN)
    {   MONO_MZ_END = IN; }

    double get_THEO_MZ()
    {   return get_best_MS2_SCAN()->get_MONO_MZ(); }
    double get_THEO_MZ(double T)
    {   return get_best_MS2_SCAN(T)->get_MONO_MZ(); }
    std::string get_AC()
    {   return get_best_MS2_SCAN()->get_AC(); }
    std::string get_AC(double T)
    {   return get_best_MS2_SCAN(T)->get_AC(); }
    bool check_AC(std::string IN)
    {   return get_best_MS2_SCAN()->compare_AC(IN); }
    bool check_AC(std::string IN, double T)
    {   return get_best_MS2_SCAN(T)->compare_AC(IN); }
    std::string get_SQ()
    {   return get_best_MS2_SCAN()->get_SQ(); }
    std::string get_SQ(double T)
    {   return get_best_MS2_SCAN(T)->get_SQ(); }
    std::string get_TOTAL_SQ()
    {   return get_best_MS2_SCAN()->get_TOTAL_SQ(); }
    std::string get_TOTAL_SQ(double T)
    {   return get_best_MS2_SCAN(T)->get_TOTAL_SQ(); }
    std::string get_MOD_SQ()
    {   return get_best_MS2_SCAN()->get_MOD_SQ(); }
    std::string get_MOD_SQ(double T)
    {   return get_best_MS2_SCAN(T)->get_MOD_SQ(); }
    double get_pep_prob()
    {   return get_best_MS2_SCAN()->get_PEP_PROB(); }
    double get_pep_prob(double T)
    {   return get_best_MS2_SCAN(T)->get_PEP_PROB(); }
    std::string get_MS2_TYPE_TAG()
    {   return get_best_MS2_SCAN()->get_MS2_TYPE_TAG(); }
    std::string get_MS2_TYPE_TAG(double T)
    {   return get_best_MS2_SCAN(T)->get_MS2_TYPE_TAG(); }
    int get_MS2_scan()
    {   return get_best_MS2_SCAN()->get_SCAN_START(); }
    int get_MS2_scan(double T)
    {   return get_best_MS2_SCAN(T)->get_SCAN_START(); }
    std::map<double, std::vector<MS2Info> > * get_MS2_SCAN_LIST()
    {   return &(MS2_SCANS); }
    std::map<double, std::vector<MS2Info> >::iterator get_MS2_SCAN_LIST_START()
    {   return MS2_SCANS.begin(); }
    std::map<double, std::vector<MS2Info> >::iterator get_MS2_SCAN_LIST_END()
    {   return MS2_SCANS.end(); }

    int get_scan_number()
    {   return scan_apex; }
    void set_scan_number(int IN)
    {   scan_apex = IN; }
    int get_scan_start()
    {   return scan_start; }
    void set_scan_start(int IN)
    {   scan_start = IN; }
    int get_scan_end()
    {   return scan_end; }
    void set_scan_end(int IN)
    {   scan_end = IN; }
    int get_charge_state()
    {   return charge_state; }
    void set_charge_state(int IN)
    {   charge_state = IN; }
    void set_peak_area(float IN)
    {   total_peak_area = IN; }
    double get_peak_area()
    {   return total_peak_area; }
    // get peak area at a certain LC/MS:
    double get_peak_area(int);
    double get_apex_peak_intensity()
    {   return apex_peak_intensity; }
    void set_apex_peak_intensity(double in)
    {   apex_peak_intensity = in; }
    void normalize_peak_area_by_factor(double factor)
    {   total_peak_area *= factor; }

    double get_alignment_error_up()
    {   return alignment_error_up; }
    void set_alignment_error_up(double IN)
    {   alignment_error_up = IN; }
    double get_alignment_error_down()
    {   return alignment_error_down; }
    void set_alignment_error_down(double IN)
    {   alignment_error_down = IN; }

    void set_SCORE_HOLDER(double IN)
    {   SCORE_HOLDER = IN; }
    double get_SCORE_HOLDER()
    {   return SCORE_HOLDER; }

    double get_retention_time()
    {   return TR; }
    void set_retention_time(double IN)
    {   TR = IN; }
    double get_retention_time_START()
    {   return TR_START; }
    void set_retention_time_START(double IN)
    {   TR_START = IN; }
    double get_retention_time_END()
    {   return TR_END; }
    void set_retention_time_END(double IN)
    {   TR_END = IN; }

    // original mz and Tr coordinates
    double get_raw_retention_time_apex()
    {   return TR_APEX; }
    void set_raw_retention_time_apex(double IN)
    {   TR_APEX = IN; }
    double get_raw_MZ()
    {   return MONO_MZ_ORIGINAL; }
    void set_raw_MZ(double IN)
    {   MONO_MZ_ORIGINAL = IN; }

    // feature ID:
    void set_feature_ID(int IN)
    {   feature_ID = IN; }
    int get_feature_ID()
    {   return feature_ID; }

    void set_spectrum_ID(int IN)
    {
      spectrum_ID = IN;
      /*
       if( MS2TraceFeature != NULL){
       MS2TraceFeature.set
       }
       */
    }

    int get_spectrum_ID()
    {   return spectrum_ID; }

    void set_MASTER_ID(int IN)
    {   MASTER_ID = IN; }
    int get_MASTER_ID()
    {   return MASTER_ID; }

    // check how many matches
    int get_nb_common_match();

    // get/set the peak score
    double get_peak_score()
    {   return PEAK_SCORE; }
    void set_peak_score(double in)
    {   PEAK_SCORE = in; }

    // get the molecular mass of the corresponding peptide!
    double get_Molecular_Mass();

    // feature PI:
    double get_FEATURE_PI()
    {   return PI; }
    void set_FEATURE_PI(double IN)
    {   PI = IN; }

    // check charge states, in cases where a feature was
    // created based on a MS2 trace, charge state is unknown ( = -1 )
    // -> derive the charge state from the matched feature (if this is
    // also not -1
    void deriveChargeStates(SHFeature *);

    // LC elution profile
    void setLCelutionProfile(FeatureLCProfile * IN)
    {   LCprofile = IN; }
    FeatureLCProfile * getLCelutionProfile()
    {   return LCprofile; }

    //////////////////////////////////////////////
    // parameters computed over matched features:
    double get_profile_retention_time();
    double get_profile_Molecular_Mass();

    /////////////////////////////////////////////
    // status if feature has been matched:
    bool get_feature_match_status()
    {   return feature_match_status; }
    void set_feature_match_status(bool IN)
    {   feature_match_status = IN; }

    ///////////////////////////////////////////
    // access the MS2 feature
    void addMS2Feature(MS2Feature * in)
    {   MS2TraceFeature = new MS2Feature(in); }
    void removeMS2Feature()
    {   delete MS2TraceFeature; MS2TraceFeature = NULL; }
    MS2Feature * getMS2Feature()
    {   return MS2TraceFeature; }

    double getSignalToNoise()
    {   return SignalToNoise; }
    void setSignalToNoise(double in)
    {   SignalToNoise = in; }

    double getBackgroundNoiseLevel()
    {   return BackgroundNoise; }
    void setBackgroundNoiseLevel(double in)
    {   BackgroundNoise = in; }

    //////////////////////////////////////////////
    // get static members:
    //static double get_MZ_TOL(){return MZ_TOL;};
    static double get_MONO_H()
    {   return _MONO_H; }

    // compare to masses at the PPM value and decided
    // if they fall into the m/z tolerance window
    static bool compareFeatureMassValuesAtPPMLevel(double, double);

    // get the masse error at the PPM value
    static double getFeatureMassErrorAtPPMLevel(double);

  };

} // ns

#endif // OPENMS_TRANSFORMATIONS_FEATUREFINDER_SUPERHIRN_SHFEATURE_H
