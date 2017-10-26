//some standard C++ includes 
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <math.h>
#include <iterator>
#include <algorithm>
#include <fstream>

//some ROOT includes
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TFile.h"
#include "TMath.h"
#include "TTree.h"
#include "TBranch.h"
#include "TVector3.h"

//"art" includes (canvas, and gallery)
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"
#include "gallery/Handle.h"

//"larsoft" object includes
#include "lardataobj/RecoBase/OpFlash.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/SpacePoint.h"
#include "lardataobj/AnalysisBase/T0.h"
#include "lardataobj/MCBase/MCTrack.h"
#include "lardataobj/MCBase/MCStep.h"

//(timing information for the beam)
#include "lardataobj/RawData/TriggerData.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h"

//"larreco" reconstruction for MCS.
#include "larreco/RecoAlg/TrajectoryMCSFitter.h"

using namespace art;
using namespace std;
using namespace std::chrono;

int main() {

  TFile f_output("/pnfs/uboone/scratch/users/abhat/nospacecharge_gallery_output.root","RECREATE");

  /************************* SCEtree Branches **********************************************************/
  // Event information.
  double event_eventNum;
  double event_subrunNum;
  double event_runNum;

  // Track & MC Track kinematic information.
  int    matched;
  double track_t0;
  double track_startX;
  double track_startY;
  double track_startZ;
  double track_endX;
  double track_endY;
  double track_endZ;
  double track_MCS_measurement;
  double track_length;
  double track_theta;
  double track_phiU;
  double track_phiV;
  double track_phiY;
  double mctrack_t0;
  double mctrack_startX;
  double mctrack_startY;
  double mctrack_startZ;
  double mctrack_endX;
  double mctrack_endY;
  double mctrack_endZ;
  double mctrack_step0_energy;
  double mctrack_step0_ptot;
  double mctrack_step0_px;
  double mctrack_step0_py;
  double mctrack_step0_pz;
  double mctrack_finalstep_energy;
  double mctrack_finalstep_ptot;
  double mctrack_finalstep_px;
  double mctrack_finalstep_py;
  double mctrack_finalstep_pz;

  // Track Classification information.
  int anode;
  int cathode;
  int enters_front;
  int enters_back;
  int enters_top;
  int exits_front;
  int exits_back;
  int exits_bottom;
  int top_front_corner;
  int top_back_corner;
  int bottom_front_corner;
  int bottom_back_corner;

  // Flash information.
  double pe_flash;
  double time_flash;
  double dt_flash;
  /************************* End SCEtree Branches **********************************************************/


  /***************************** Start User-Adjustable Parameters ******************************************/
  bool IsMC;
  bool verbose;
  /***************************** End User-Adjustable   Parameters ********************************************/


  /***************************** Fixed Parameters ********************************************/
  double MCRes;
  double TPCRes;
  double flash_time_res;
  double _TOP; double _BOTTOM; double _FRONT; double _BACK;
  /***************************** End Fixed Parameters ******************************************/


  /***************************** ADC Truncation Offset Values  ********************************************/
  double anode_offset_mc;
  double cathode_offset_mc;
  double anode_offset_data;
  double cathode_offset_data;

  double neg_min_ADC_cut;
  double neg_max_ADC_cut;
  double pos_min_ADC_cut;
  double pos_max_ADC_cut;
  /***************************** End ADC Truncation Offset Values  ********************************************/


  /***************************** MCS Algorithm Values *****************************************************/
  int     pIdHypothesis;
  int     minNumSegments;
  double  segmentLength;
  int     minHitsPerSegment;
  int     nElossSteps;
  int     eLossMode;
  double  pMin;
  double  pMax;
  double  pStep;
  double  angResol;
  /**************************** End of MCS Algorithm Values ***********************************************/


  /***************************** Track Classification Functions ********************************************/
  bool MatchTracks(const recob::Track& track, const sim::MCTrack& mctrk, const double& res);
  bool TrackEntersTop(const std::vector<TVector3>& sorted_trk, double _TOP);
  bool TrackEntersFront(const std::vector<TVector3>& sorted_trk, double _FRONT);
  bool TrackEntersBack(const std::vector<TVector3>& sorted_trk, double _BACK);
  bool TrackEntersAnode(const std::vector<TVector3>& sorted_trk);
  bool TrackEntersCathode(const std::vector<TVector3>& sorted_trk);
  bool TrackExitsBottom(const std::vector<TVector3>& sorted_trk, double _BOTTOM);
  bool TrackExitsFront(const std::vector<TVector3>& sorted_trk, double _FRONT);
  bool TrackExitsBack(const std::vector<TVector3>& sorted_trk, double _BACK);
  bool TrackExitsAnode(const std::vector<TVector3>& sorted_trk);
  bool TrackExitsCathode(const std::vector<TVector3>& sorted_trk);
  void SortTrackPoints(const recob::Track& track, std::vector<TVector3>& sorted_trk);
  /***************************** End Track Classification Functions ********************************************/


  /***************************** SCE Tree Initialization ******************************************************/
  TTree* SCEtree = new TTree("SCEtree","SCEtree");  
  SCEtree->Branch("event_eventNum",&event_eventNum,"event_eventNum/D");
  SCEtree->Branch("event_subrunNum",&event_subrunNum,"event_subrunNum/D");
  SCEtree->Branch("event_runNum",&event_runNum,"event_runNum/D");
  SCEtree->Branch("matched",&matched,"matched/I");
  SCEtree->Branch("track_t0",&track_t0,"track_t0/D");
  SCEtree->Branch("track_startX",&track_startX,"track_startX/D");
  SCEtree->Branch("track_startY",&track_startY,"track_startY/D");
  SCEtree->Branch("track_startZ",&track_startZ,"track_startZ/D");
  SCEtree->Branch("track_endX",&track_endX,"track_endX/D");
  SCEtree->Branch("track_endY",&track_endY,"track_endY/D");
  SCEtree->Branch("track_endZ",&track_endZ,"track_endZ/D");
  SCEtree->Branch("track_MCS_measurement",&track_MCS_measurement, "track_MCS_measurement/D");
  SCEtree->Branch("track_length",&track_length,"track_length/D");
  SCEtree->Branch("track_theta",&track_theta,"track_theta/D");
  SCEtree->Branch("track_phiU",&track_phiU,"track_phiU/D");
  SCEtree->Branch("track_phiV",&track_phiV,"track_phiV/D");
  SCEtree->Branch("track_phiY",&track_phiY,"track_phiY/D");
  SCEtree->Branch("mctrack_t0",&mctrack_t0,"mctrack_t0/D");
  SCEtree->Branch("mctrack_startX",&mctrack_startX,"mctrack_startX/D");
  SCEtree->Branch("mctrack_startY",&mctrack_startY,"mctrack_startY/D");
  SCEtree->Branch("mctrack_startZ",&mctrack_startZ,"mctrack_startZ/D");
  SCEtree->Branch("mctrack_endX",&mctrack_endX,"mctrack_endX/D");
  SCEtree->Branch("mctrack_endY",&mctrack_endY,"mctrack_endY/D");
  SCEtree->Branch("mctrack_endZ",&mctrack_endZ,"mctrack_endZ/D");
  SCEtree->Branch("mctrack_step0_energy", &mctrack_step0_energy, "mctrack_step0_energy/D");
  SCEtree->Branch("mctrack_step0_ptot", &mctrack_step0_ptot, "mctrack_step0_ptot/D");
  SCEtree->Branch("mctrack_step0_px", &mctrack_step0_px, "mctrack_step0_px/D");
  SCEtree->Branch("mctrack_step0_py", &mctrack_step0_py, "mctrack_step0_py/D");
  SCEtree->Branch("mctrack_step0_pz", &mctrack_step0_pz, "mctrack_step0_pz/D");
  SCEtree->Branch("mctrack_finalstep_energy", &mctrack_finalstep_energy, "mctrack_finalstep_energy/D");
  SCEtree->Branch("mctrack_finalstep_ptot", &mctrack_finalstep_ptot, "mctrack_finalstep_ptot/D");
  SCEtree->Branch("mctrack_finalstep_px", &mctrack_finalstep_px, "mctrack_finalstep_px/D");
  SCEtree->Branch("mctrack_finalstep_py", &mctrack_finalstep_py, "mctrack_finalstep_py/D");
  SCEtree->Branch("mctrack_finalstep_pz", &mctrack_finalstep_pz, "mctrack_finalstep_pz/D");
  SCEtree->Branch("anode", &anode, "anode/I");
  SCEtree->Branch("cathode", &cathode, "cathode/I");
  SCEtree->Branch("enters_front", &enters_front, "enters_front/I");
  SCEtree->Branch("enters_back", &enters_back, "enters_back/I");
  SCEtree->Branch("enters_top", &enters_top, "enters_top/I");
  SCEtree->Branch("exits_front", &exits_front, "exits_front/I");
  SCEtree->Branch("exits_back", &exits_back, "exits_back/I");
  SCEtree->Branch("exits_bottom", &exits_bottom, "exits_bottom/I");
  SCEtree->Branch("top_front_corner", &top_front_corner, "top_front_corner/I");
  SCEtree->Branch("top_back_corner", &top_back_corner, "top_back_corner/I");
  SCEtree->Branch("bottom_front_corner", &bottom_front_corner, "bottom_front_corner/I");
  SCEtree->Branch("bottom_back_corner", &bottom_back_corner, "bottom_back_corner/I");
  SCEtree->Branch("time_flash",&time_flash,"time_flash/D");
  SCEtree->Branch("pe_flash",&pe_flash,"pe_flash/D");
  SCEtree->Branch("dt_flash",&dt_flash,"dt_flash/D");
  /***************************** End SCE Tree Initialization ******************************************************/


  vector<string> filenames;
  ifstream myfile("/uboone/app/users/abhat/larsoft_mydev/srcs/chris/nospacechargefile_list.txt");
  copy(istream_iterator<string>(myfile),istream_iterator<string>(),back_inserter(filenames));

  /***************************** Parameter Initialization Section  ********************************************/


  /***************************** User Parameter Initialization Section *******************************************/  
  // These are the ONLY parameters that the user is free to change.
  IsMC            = true; // Must be set to 'false' when running on data to incorporate ACD truncation cut & Reco/Truth Matching.
  verbose         = false;
  /***************************** End of User Parameter Initialization Section *************************************/

  /***************************** SCE-Specific Parameter Initialization (Should Not Be Changed Without Significant Changes to Analysis) ******************/
  MCRes           = 20.;
  TPCRes          = 20.;
  flash_time_res  = 2.;

  // Set the values of the TPC boundaries here according to your resolution value (here we use 20.0 cm).                                                                                               
  _TOP    = 116.5 - TPCRes;
  _BOTTOM = -116.5 + TPCRes;
  _FRONT  = TPCRes;
  _BACK   = 1036.8 - TPCRes;

  if (verbose == true) {
    // Print all of these values as a sanity check.                                                                                                                                                   
    std::cout << "The top of the TPC is located at " << _TOP << "." << std::endl;
    std::cout << "The bottom of the TPC is located at " << _BOTTOM << "." << std::endl;
    std::cout << "The front of the TPC is located at " << _FRONT << "." << std::endl;
    std::cout << "The back of the TPC is located at " << _BACK << "." << std::endl;
    std::cout << "\n" << std::endl;
  }
  /**************************** End of SCE-Specific Parameter Initialization *******************************/


  /**************************** Flash-Time Offsets Used in 'pandoraCosmicT0Reco' module **************/
  anode_offset_mc     = -0.53;
  cathode_offset_mc   = 9.97; 
  anode_offset_data   = 5.0;
  cathode_offset_data = -16.4;

  neg_min_ADC_cut     = -406.;
  neg_max_ADC_cut     = -400.;
  pos_min_ADC_cut     = 500.;
  pos_max_ADC_cut     = 507.;
  /*************************** End of Flash-Time Offsets Used in 'pandoraCosmicT0Reco' module ***********/

  /*************************** MCS Algorithm Values Initialization *****************************************/
  pIdHypothesis       = 13;
  minNumSegments      = 3;
  segmentLength       = 14.;
  minHitsPerSegment   = 2;
  nElossSteps         = 10;
  eLossMode           = 0; // Set to 'MPV Landau' mode currently ('eLossMode' = 0).
  pMin                = 0.01;
  pMax                = 7.50;
  pStep               = 0.01;
  angResol            = 3.0;
  /************************** End MCS Algorithm Values Initialization **************************************/


  /***************************** End of Parameter Initialization Section ********************************************/


  InputTag rawdigit_tag { "daq" };
  InputTag hit_tag { "gaushit" };
  InputTag track_tag { "pandoraCosmic" };
  InputTag t0_tag { "pandoraCosmicT0Reco" };
  InputTag flash_tag { "pandoraCosmicT0Reco" };

  int totalEventNum = 0;

  for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {
    auto t_begin = high_resolution_clock::now();
    totalEventNum++;

    event_eventNum = ev.eventAuxiliary().event();
    event_subrunNum = ev.eventAuxiliary().subRun();
    event_runNum = ev.eventAuxiliary().run();

    cout << "Processing "
	 << "Run " << event_runNum << ", "
	 << "Subun " << event_subrunNum << ", "
	 << "Event " << event_eventNum << endl;

    // Track Handle Information in the loop.
    auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
    auto const& track_vec(*track_handle);

    /************ MC Track Information *******************/
    // Load the information for the MC track information.
    gallery::Handle<std::vector<sim::MCTrack> > mctrk_h;
    ev.getByLabel("mcreco",mctrk_h);

    // Check to make sure that the collection of Monte Carlo tracks looks good.
    // if we should use MCTrack                                                   
    if (IsMC){
      // make sure tracks look good                                               
      if(!mctrk_h.isValid()) {
	std::cerr<<"\033[93m[ERROR]\033[00m ... could not locate MCTrack!"<<std::\
	  endl;
	throw std::exception();
      }
    }// if use MCTrack   
    /************ End MC Track Information *******************/


    FindMany<recob::Hit> hits_per_track(track_handle,ev,track_tag);
    FindMany<anab::T0> trk_t0_assn_v(track_handle,ev,t0_tag);
    FindMany<recob::OpFlash> trk_flash_assn_v(track_handle, ev, flash_tag );
    
    for (size_t i_t = 0, size_track = track_handle->size(); i_t != size_track; ++i_t) {
      track_length          = track_vec.at(i_t).Length();
      trkf::TrajectoryMCSFitter MCSFitter(pIdHypothesis, minNumSegments, segmentLength, minHitsPerSegment, nElossSteps, eLossMode, pMin, pMax, pStep, angResol);
      const recob::MCSFitResult fit_result = MCSFitter.fitMcs(track_vec.at(i_t));
      track_MCS_measurement = fit_result.fwdMomentum()*1000; // Express in units of MeV.
      TVector3 track_startdir = (TVector3) track_vec.at(i_t).VertexDirection();
      TVector3 track_enddir = (TVector3) track_vec.at(i_t).EndDirection();
      std::vector<const anab::T0*> T0_v = trk_t0_assn_v.at(i_t);
      std::vector<const recob::OpFlash*> flash_v = trk_flash_assn_v.at(i_t);

      // Change the direction of the track and its MCS momentum if the reconstructed track is not downwards-going.
      if ( track_vec.at(i_t).LocationAtPoint( 0 ).Y() < track_vec.at(i_t).LocationAtPoint( track_vec.at(i_t).NumberTrajectoryPoints() - 1 ).Y() ) {
        TVector3 track_startdir = (TVector3) track_vec.at(i_t).EndDirection();
        TVector3 track_enddir = (TVector3) track_vec.at(i_t).VertexDirection();
        track_MCS_measurement = fit_result.bwdMomentum()*1000; 
      }


      /***************************** Track Classification ********************************************/
      // 'SortTrackPoints' orients the track points to be downwards-going.  From this point forward, pt[0] is the point determined by the algorithm to be the starting point of the track, and the pt[track_vec.size() - 1] is the ending point.
      std::vector<TVector3> sorted_trk;
      SortTrackPoints(track_vec.at(i_t),sorted_trk);
      
      // Makes sure that only one reconstruction case is registered (for the corner cases).
      anode = cathode = enters_front = enters_back = enters_top = exits_front = exits_back = exits_bottom = top_front_corner = top_back_corner = bottom_front_corner = bottom_back_corner = 0;

      // Purely endcap-piercing tracks.
      if ( (TrackExitsFront(sorted_trk, _FRONT)   == true)   && (TrackExitsBottom(sorted_trk, _BOTTOM) == false) && (TrackEntersAnode(sorted_trk)    == true) )  { anode = 1;   exits_front = 1;  if (verbose) std::cout << "anode->front" << std::endl; }
      if ( (TrackExitsFront(sorted_trk, _FRONT)   == true)   && (TrackExitsBottom(sorted_trk, _BOTTOM) == false) && (TrackEntersCathode(sorted_trk)  == true) )  { cathode = 1; exits_front = 1;  if (verbose) std::cout << "cathode->front" << std::endl;}
      if ( (TrackExitsBack(sorted_trk, _BACK)     == true)   && (TrackExitsBottom(sorted_trk, _BOTTOM) == false) && (TrackEntersAnode(sorted_trk)    == true) )  { anode = 1;   exits_back = 1;   if (verbose) std::cout << "anode->back" << std::endl;}
      if ( (TrackExitsBack(sorted_trk, _BACK)     == true)   && (TrackExitsBottom(sorted_trk, _BOTTOM) == false) && (TrackEntersCathode(sorted_trk)  == true) )  { cathode = 1; exits_back = 1;   if (verbose) std::cout << "cathode->back" << std::endl; }
      if ( (TrackEntersFront(sorted_trk, _FRONT)  == true)   && (TrackEntersTop(sorted_trk, _TOP) == false)      && (TrackExitsAnode(sorted_trk)     == true) )  { anode = 1;   enters_front = 1; if (verbose) std::cout << "front->anode" << std::endl;}
      if ( (TrackEntersFront(sorted_trk, _FRONT)  == true)   && (TrackEntersTop(sorted_trk, _TOP) == false)      && (TrackExitsCathode(sorted_trk)   == true) )  { cathode = 1; enters_front = 1; if (verbose) std::cout << "front->cathode" << std::endl;}											       
      if ( (TrackEntersBack(sorted_trk, _BACK)    == true)   && (TrackEntersTop(sorted_trk, _TOP) == false)      && (TrackExitsAnode(sorted_trk)     == true) )  { anode = 1;   enters_back = 1;  if (verbose) std::cout << "back->anode" << std::endl; }

      if ( (TrackEntersBack(sorted_trk, _BACK)    == true)   && (TrackEntersTop(sorted_trk, _TOP) == false)      && (TrackExitsCathode(sorted_trk)   == true) )  { cathode = 1; enters_back = 1;  if (verbose) std::cout << "back->cathode" << std::endl;}

      // Purely top-and-bottom-piercing tracks.
      if ( (TrackExitsBottom(sorted_trk, _BOTTOM) == true)   && (TrackExitsFront(sorted_trk, _FRONT) == false)   && (TrackExitsBack(sorted_trk, _BACK) == false)  && (TrackEntersAnode(sorted_trk)    == true) ) { anode = 1;   exits_bottom = 1; if (verbose) std::cout << "anode->bottom" << std::endl;}													    
      if ( (TrackExitsBottom(sorted_trk, _BOTTOM) == true)   && (TrackExitsFront(sorted_trk, _FRONT) == false)   && (TrackExitsBack(sorted_trk, _BACK) == false)  && (TrackEntersCathode(sorted_trk)  == true) ) { cathode = 1; exits_bottom = 1; if (verbose) std::cout << "cathode->bottom" << std::endl;}
      if ( (TrackEntersTop(sorted_trk, _TOP)      == true)   && (TrackEntersFront(sorted_trk, _FRONT) == false)  && (TrackEntersBack(sorted_trk, _BACK) == false) && (TrackExitsAnode(sorted_trk)     == true) ) { anode = 1;   enters_top = 1;   if (verbose) std::cout << "top->anode" << std::endl;}
      if ( (TrackEntersTop(sorted_trk, _TOP)      == true)   && (TrackEntersFront(sorted_trk, _FRONT) == false)  && (TrackEntersBack(sorted_trk, _BACK) == false) && (TrackExitsCathode(sorted_trk)   == true) ) { cathode = 1; enters_top = 1;   if (verbose) std::cout << "top->cathode" << std::endl;}

      // Corner cases.
      if ( (TrackEntersTop(sorted_trk, _TOP) == true)        && (TrackEntersFront(sorted_trk, _FRONT) == true)   && (TrackExitsAnode(sorted_trk) == true) )    { anode = 1;    top_front_corner = 1;    if (verbose) std::cout << "top/front corner->anode" << std::endl;}
      if ( (TrackEntersTop(sorted_trk, _TOP) == true)        && (TrackEntersFront(sorted_trk, _FRONT) == true)   && (TrackExitsCathode(sorted_trk) == true) )  { cathode = 1;  top_front_corner = 1;    if (verbose) std::cout << "top/front corner->cathode" << std::endl;}
      if ( (TrackEntersTop(sorted_trk, _TOP) == true)        && (TrackEntersBack(sorted_trk, _BACK) == true)     && (TrackExitsAnode(sorted_trk)  == true) )   { anode = 1;    top_back_corner = 1;     if (verbose) std::cout << "top/back corner->anode" << std::endl;}
      if ( (TrackEntersTop(sorted_trk, _TOP) == true)        && (TrackEntersBack(sorted_trk, _BACK) == true)     && (TrackExitsCathode(sorted_trk) == true) )  { cathode = 1;  top_back_corner = 1;     if (verbose) std::cout << "top/back corner->cathode" << std::endl;}
      if ( (TrackExitsBottom(sorted_trk, _BOTTOM) == true)   && (TrackExitsFront(sorted_trk, _FRONT) == true)    && (TrackEntersAnode(sorted_trk)  == true) )  { anode = 1;    bottom_front_corner = 1; if (verbose) std::cout << "bottom/front corner->anode" << std::endl;}
      if ( (TrackExitsBottom(sorted_trk, _BOTTOM) == true)   && (TrackExitsFront(sorted_trk, _FRONT) == true)    && (TrackEntersCathode(sorted_trk) == true) ) { cathode = 1;  bottom_front_corner = 1; if (verbose) std::cout << "bottom/front corner->cathode" << std::endl;}
      if ( (TrackExitsBottom(sorted_trk, _BOTTOM) == true)   && (TrackExitsBack(sorted_trk, _BACK)   == true)    && (TrackEntersAnode(sorted_trk)  == true) )  { anode = 1;    bottom_back_corner = 1;  if (verbose) std::cout << "bottom/back corner->anode" << std::endl;}
      if ( (TrackExitsBottom(sorted_trk, _BOTTOM) == true)   && (TrackExitsBack(sorted_trk, _BACK) == true)      && (TrackEntersCathode(sorted_trk) == true) ) { cathode = 1;  bottom_back_corner = 1;  if (verbose) std::cout << "bottom/back corner->cathode" << std::endl;}
      /***************************** End Track Classification ********************************************/

      if (T0_v.size() != 1) continue;
      auto recot0 = T0_v.at(0);
      track_t0 = recot0->Time();

      /***************************** ADC Time Offset Cut  ********************************************/
      // Set a value for the offset to avoid having to write more code later.
      double data_offset = 0.;
      double mc_offset   = 0.;
      if ( anode == 1   && IsMC == false )  {
	data_offset = anode_offset_data;   
	mc_offset = anode_offset_mc;
      }
      if ( cathode == 1 && IsMC == false )   {
	data_offset = cathode_offset_data; 
	mc_offset = cathode_offset_mc;
      }

      // Include the cut on the truncated ADC waveforms within this section - only necessary for the tracks in data.
      // This cut is because of the current state of production, which does not include properly centered ADC truncation cuts on data.
      if ( IsMC == false ) {

	if ( (track_t0 > ( neg_min_ADC_cut - ( mc_offset - data_offset ) ) && track_t0 < ( neg_max_ADC_cut - ( mc_offset -  data_offset ) ) ) || ( track_t0 > ( pos_min_ADC_cut - ( mc_offset - data_offset ) ) && track_t0 < ( pos_max_ADC_cut - ( mc_offset - data_offset ) ) ) ) {

	continue;

	}
      }
      /***************************** End of ADC Time Offset Cut ********************************************/

      /***************************** Flash Time Info & Cut Section ****************************************/
      // Fill the information for the reconstructed flash.
      if (flash_v.size() != 1) continue;      
      auto flash = flash_v.at(0);
      pe_flash    = flash->TotalPE();
      time_flash  = flash->Time();
      dt_flash    = fabs(track_t0 - time_flash);

      // Impose the cut on the flash time using 'flash_time_res'.
      if ( dt_flash > flash_time_res ) continue;
      /*************************** End of Flash Time Info & Cut Section **********************************/
      

      /*************************** Monte Carlo Matching Section *************************************************************/
      matched = 0;

      if (IsMC == true) {

	// loop through MCTracks to find the one that matches.                  
	for (size_t j=0; j < mctrk_h->size(); j++){

	  auto const& mctrk = mctrk_h->at(j);

	  // try matching to MC                                                                                                                                                              
	  if ( MatchTracks(track_vec.at(i_t), mctrk, MCRes) == false )
            continue;

          // matched -> get MCTrack time and reconstructed track reconstructed T0                                                                                                              
	  mctrack_t0                 = mctrk.at(0).T() / 1000.;
          matched                    = 1;

          // Here is where I'll set the coordinates for the start and end of the mc tracks                                                                                                        
	  mctrack_startX             = mctrk.at(0).X();
          mctrack_startY             = mctrk.at(0).Y();
          mctrack_startZ             = mctrk.at(0).Z();
          mctrack_endX               = mctrk.at(mctrk.size() - 1).X();
          mctrack_endY               = mctrk.at(mctrk.size() - 1).Y();
          mctrack_endZ               = mctrk.at(mctrk.size() - 1).Z();

	  // Set the mctrack energy and momentum.  This will be based on which endpoint has the greatest y-coordinate.
	  mctrack_step0_energy       = mctrk.at(0).E();
	  mctrack_step0_px           = mctrk.at(0).Px();
	  mctrack_step0_py           = mctrk.at(0).Py();
	  mctrack_step0_pz           = mctrk.at(0).Pz();

	  mctrack_finalstep_energy   = mctrk.at(mctrk.size() - 1).E();
	  mctrack_finalstep_px       = mctrk.at(mctrk.size() - 1).Px();
	  mctrack_finalstep_py       = mctrk.at(mctrk.size() - 1).Py();
	  mctrack_finalstep_pz       = mctrk.at(mctrk.size() - 1).Pz();

	  // If the 'mctrack' is not downwards-going, then re-orient the start and end points so that it now is.
	  if ( mctrack_endY > mctrack_startY ) {
	    mctrack_startX           = mctrk.at(mctrk.size() - 1).X();
	    mctrack_startY           = mctrk.at(mctrk.size() - 1).Y();
	    mctrack_startZ           = mctrk.at(mctrk.size() - 1).Z();

	    mctrack_endX             = mctrk.at(0).X();
	    mctrack_endY             = mctrk.at(0).Y();
	    mctrack_endZ             = mctrk.at(0).Z();

	    mctrack_step0_energy     = mctrk.at(mctrk.size() - 1).E();
	    mctrack_step0_px         = mctrk.at(mctrk.size() - 1).Px();
	    mctrack_step0_py         = mctrk.at(mctrk.size() - 1).Py();
	    mctrack_step0_pz         = mctrk.at(mctrk.size() - 1).Pz();

	    mctrack_finalstep_energy = mctrk.at(0).E();
	    mctrack_finalstep_px     = mctrk.at(0).Px();
	    mctrack_finalstep_py     = mctrk.at(0).Py();
	    mctrack_finalstep_pz     = mctrk.at(0).Pz();
	  }

	  // Set these below, which are dependent on the values of the initial & final momentum (after those have been set).
	  mctrack_step0_ptot         = sqrt( mctrack_step0_px * mctrack_step0_px + mctrack_step0_py * mctrack_step0_py + mctrack_step0_pz * mctrack_step0_pz );
	  mctrack_finalstep_ptot     = sqrt( mctrack_finalstep_px * mctrack_finalstep_px + mctrack_finalstep_py * mctrack_finalstep_py + mctrack_finalstep_pz * mctrack_finalstep_pz );

	  if (verbose == true) {
	    std::cout << "\n" << std::endl;
	    std::cout << "Starting a new track." << std::endl;
	  }

	} // for all MCTracks                                                                                                                                                                               
      }// if we should use MCTracks                                                                                                                                                                   

      // If the track is not matched and this is the MC run, then continue.
      if ( IsMC == true && matched == 0 ) 
	continue;
      /*************************** End Monte Carlo Matching Section *************************************************************/
      
      /*************************** Fill the rest of the track kinematic information, after all cuts & track classification.  *********************************************/
      track_startX = sorted_trk.at(0)[0] - .114*track_t0;
      track_startY = sorted_trk.at(0)[1];
      track_startZ = sorted_trk.at(0)[2];

      track_endX = sorted_trk.at( sorted_trk.size() - 1 )[0] - .114*track_t0;
      track_endY = sorted_trk.at( sorted_trk.size() - 1 )[1];
      track_endZ = sorted_trk.at( sorted_trk.size() - 1 )[2];

      TVector3 track_dir;
      if(sorted_trk.at(0)[0] < sorted_trk.at( sorted_trk.size() - 1 )[0]) {
        track_dir[0] = track_startdir[1];
        track_dir[1] = track_startdir[2];
        track_dir[2] = track_startdir[0];
      }
      else {
        track_dir[0] = track_enddir[1];
        track_dir[1] = track_enddir[2];
        track_dir[2] = track_enddir[0];
      }
    
      track_theta = 3.141593/2.0 - TMath::Min(track_dir.Theta(),3.141593-track_dir.Theta());
      track_phiU = track_dir.Phi() - 3.141593/3.0;
      track_phiV = track_dir.Phi() + 3.141593/3.0;
      track_phiY = track_dir.Phi();
    
      if(track_phiU > 2.5*3.141593) {
        track_phiU -= 3.0*3.141593;
      }
      else if(track_phiU > 1.5*3.141593) {
        track_phiU -= 2.0*3.141593;
      }
      else if(track_phiU > 0.5*3.141593) {
        track_phiU -= 1.0*3.141593;
      }
      else if(track_phiU < -2.5*3.141593) {
        track_phiU += 3.0*3.141593;
      }
      else if(track_phiU < -1.5*3.141593) {
        track_phiU += 2.0*3.141593;
      }
      else if(track_phiU < -0.5*3.141593) {
        track_phiU += 1.0*3.141593;
      }
      track_phiU = fabs(track_phiU);
    
      if(track_phiV > 2.5*3.141593) {
        track_phiV -= 3.0*3.141593;
      }
      else if(track_phiV > 1.5*3.141593) {
        track_phiV -= 2.0*3.141593;
      }
      else if(track_phiV > 0.5*3.141593) {
        track_phiV -= 1.0*3.141593;
      }
      else if(track_phiV < -2.5*3.141593) {
        track_phiV += 3.0*3.141593;
      }
      else if(track_phiV < -1.5*3.141593) {
        track_phiV += 2.0*3.141593;
      }
      else if(track_phiV < -0.5*3.141593) {
        track_phiV += 1.0*3.141593;
      }
      track_phiV = fabs(track_phiV);
    
      if(track_phiY > 2.5*3.141593) {
        track_phiY -= 3.0*3.141593;
      }
      else if(track_phiY > 1.5*3.141593) {
        track_phiY -= 2.0*3.141593;
      }
      else if(track_phiY > 0.5*3.141593) {
        track_phiY -= 1.0*3.141593;
      }
      else if(track_phiY < -2.5*3.141593) {
        track_phiY += 3.0*3.141593;
      }
      else if(track_phiY < -1.5*3.141593) {
        track_phiY += 2.0*3.141593;
      }
      else if(track_phiY < -0.5*3.141593) {
        track_phiY += 1.0*3.141593;
      }
      track_phiY = fabs(track_phiY);
      /*************************** End of the filling of track orientation information.  *********************************************/
    
      SCEtree->Fill();
    }
    
    auto t_end = high_resolution_clock::now();
    duration<double,std::milli> time_total_ms(t_end-t_begin);
    cout << "\tEvent took " << time_total_ms.count() << " ms to process." << endl;
  }

  f_output.Write();
  f_output.Close();

}

/*************************** Monte Carlo Matching Function *************************************************************/
// The function for 'matching' the data track to a Monte Carlo truth track.                                                                                                                  
bool MatchTracks(const recob::Track& track, const sim::MCTrack& mctrk, const double& res)
{
  //assumes both the reco track                                                                                                                                                                     
  //and mctrack are downwards going                                                                                                                                                                  
  if ( (mctrk.size() < 2) or (track.NumberTrajectoryPoints() < 2) )
	return false;

   auto const& mctrk_s = mctrk.at(0);
   auto const& mctrk_e = mctrk.at( mctrk.size() - 1 );
   auto const& track_s = track.Vertex();
   auto const& track_e = track.End();

   // if track start is above and mctrk start is above                                                                                                                                                 
   if ( ( track_s.Y() > track_e.Y() ) and ( mctrk_s.Y() > mctrk_e.Y() ) ){
     if ( (fabs(mctrk_s.Y()-track_s.Y()) < res) and (fabs(mctrk_s.Z()-track_s.Z()) < res) and (fabs(mctrk_e.Y()-track_e.Y()) < res) and (fabs(mctrk_e.Z()-track_e.Z()) < res) )
       return true;
   }
   // if tarck start is above and mctrk start is below                                                                                                                                                
   if ( ( track_s.Y() > track_e.Y() ) and ( mctrk_s.Y() < mctrk_e.Y() ) ){
     if ( (fabs(mctrk_e.Y()-track_s.Y()) < res) and (fabs(mctrk_e.Z()-track_s.Z()) < res) and (fabs(mctrk_s.Y()-track_e.Y()) < res) and (fabs(mctrk_s.Z() - track_e.Z()) < res) )
       return true;
   }
   // if track start is below and mctrk start is above                                                                                                                                             
   if ( ( track_s.Y() < track_e.Y() ) and ( mctrk_s.Y() > mctrk_e.Y() ) ){
     if ( (fabs(mctrk_s.Y()-track_e.Y()) < res) and (fabs(mctrk_s.Z()-track_e.Z()) < res) and (fabs(mctrk_e.Y()-track_s.Y()) < res) and (fabs(mctrk_e.Z()-track_s.Z()) < res) )
       return true;
   }
   // if track start is below and mctrk start is below                                                                                                                                               
   if ( ( track_s.Y() < track_e.Y() ) and ( mctrk_s.Y() < mctrk_e.Y() ) ){
     if ( (fabs(mctrk_e.Y()-track_e.Y()) < res) and (fabs(mctrk_e.Z()-track_e.Z()) < res) and (fabs(mctrk_s.Y() - track_s.Y()) < res) and (fabs(mctrk_s.Z()-track_s.Z()) < res) )
       return true;
   }
   
   return false;
}
/*************************** End Monte Carlo Matching Function *************************************************************/


/*************************** Track Classification Functions Definitions  *************************************************************/
bool TrackEntersTop(const std::vector<TVector3>& sorted_trk, double _TOP)
{

  // check that the first point in the track                                                                                                                                                        
  // pierces the top boundary of the TPC                                                                                                                                                            
  // This track either will pierce the top of the TPC or is just about to (the '_TOP' variable is just below the actual coordinate position of the top in Y)                                       
  if (sorted_trk.at(0).Y() > _TOP)
    return true;
  
  return false;
}


bool TrackEntersFront(const std::vector<TVector3>& sorted_trk, double _FRONT)
{

  auto const& top_pt = sorted_trk.at(0);

  if (top_pt.Z() < _FRONT)
    return true;
  
  return false;
}


bool TrackEntersBack(const std::vector<TVector3>& sorted_trk, double _BACK)
{
  auto const& top_pt    = sorted_trk.at(0);

  if (top_pt.Z() > _BACK)
    return true;

  return false;
}
 


bool   TrackEntersAnode(const std::vector<TVector3>& sorted_trk)
{

  // we know the track enters either the                                                                                                                                             
  // anode or cathode                                                                                                                                                       
  // at this point figure out                                                                                                                                                                   
  // if it ENTERS the ANODE or CATHODE                                                                                                                   
  // ANODE: top point must be at lower X-coord                                                                                                                                       
  // than bottom point                                                                                                                                         
  // CATHODE: top point must be at larger X-coord                                                                                                                            
  // than bottom point                                                                                                                                          
  // assume track has already been sorted                                                                                                  
  // such that the 1st point is the most elevated in Y coord.                                                                                                        
  // return TRUE if passes the ANODE                                                                                                                                                                 
   
  auto const& top    = sorted_trk.at(0);
  auto const& bottom = sorted_trk.at( sorted_trk.size() - 1 );
  
  if (top.X() < bottom.X())
    return true;
  
  return false;
}

// Place all of the additional functions here.                                                                                                                                             
bool  TrackEntersCathode(const std::vector<TVector3>& sorted_trk)
{

  // we know the track enters either the                                                                                                                                             
  // anode or cathode                                                                                                                                                                             
  // at this point figure out                                                                                                                                                                     
  // if it ENTERS the ANODE or CATHODE                                                                                                                                                         
  // ANODE: top point must be at lower X-coord                                                                                                                                      
  // than bottom point                                                                                                                                                                     
  // CATHODE: top point must be at larger X-coord                                                                                                                                 
  // than bottom point                                                                                                                                                                     
  // assume track has already been sorted                                                                                                                                                           
  // such that the 1st point is the most elevated in Y coord.                                                                                                                                  
  // return TRUE if passes the ANODE                                                                                                                                                                        

  auto const& top    = sorted_trk.at(0);
  auto const& bottom = sorted_trk.at( sorted_trk.size() - 1 );

  if (top.X() > bottom.X())
    return true;
  
  return false;
}



bool   TrackExitsBottom(const std::vector<TVector3>& sorted_trk, double _BOTTOM)
{

  // check that the last point in the track                                                                                                                  
  // pierces the bottom boundary of the TPC                                                                                                                                                           
  if ( sorted_trk.at( sorted_trk.size() - 1).Y() < _BOTTOM )
    return true;
  
  return false;
}

bool   TrackExitsFront(const std::vector<TVector3>& sorted_trk, double _FRONT)
{
  auto const& bottom_pt = sorted_trk.at(sorted_trk.size() - 1);
   
  if (bottom_pt.Z() < _FRONT)
    return true;
  
  return false;
}

bool   TrackExitsBack(const std::vector<TVector3>& sorted_trk, double _BACK)
{
  auto const& bottom_pt = sorted_trk.at(sorted_trk.size() - 1);

  if (bottom_pt.Z() > _BACK)
    return true;

  return false;
}


bool   TrackExitsAnode(const std::vector<TVector3>& sorted_trk)
{

  // Check, once it's known that the track doesn't exit out of the bottom, whether it's the anode or                                                                                                
  // the cathode that it exits out of                                                                                                                                                                
  // This can be done by direct analogy with the 'Anode' function (shown in this file as the 'TrackEntersAnode') function written by D. Caratelli                                                    
  // Define 'top' as the point at the start of the track, and 'bottom' as the point at the end of the track                                                                                          
  
  auto const& top    = sorted_trk.at(0);
  auto const& bottom = sorted_trk.at(sorted_trk.size() - 1);

  // Check to see which point has a lower x coordinate                                                                                                                                              
  // If the bottom does, then it exits out of the anode                                                                                                                                           
  
  // If the top does, then it exits out of the cathode                                                                                                                                               
  
  if (bottom.X() < top.X())
     return true;

  return false; // Otherwise, the top is less than the bottom, so the track ended closer to the cathode and exited there                                                                                    
}

bool  TrackExitsCathode(const std::vector<TVector3>& sorted_trk)
{

  auto const& top    = sorted_trk.at(0);
  auto const& bottom = sorted_trk.at( sorted_trk.size() - 1 );
  
  if (top.X() < bottom.X())
    return true;

  return false;
}


// The function for orienting the track trajectory points to be downwards-going.
void   SortTrackPoints(const recob::Track& track, std::vector<TVector3>& sorted_trk)
{

  // vector to store 3D coordinates of                                                                                                                                              
  // ordered track                                                                                                                                                                  
  sorted_trk.clear();
  
  // take the reconstructed 3D track                                                                                                                                     
  // and assuming it is downwards                                                                                                                                   
  // going, sort points so that                                                                                                                                    
  // the track starts at the top                                                                                                                          
  // which point is further up in Y coord?                                                                                                                                     
  // start or end?                                                                                                                                                                                 
  auto const&N = track.NumberTrajectoryPoints();
  auto const&start = track.LocationAtPoint(0);
  auto const&end   = track.LocationAtPoint( N - 1 );

  // if points are ordered correctly                                                                                                                                         
  if (start.Y() > end.Y()){
    for (size_t i=0; i < N; i++)
      sorted_trk.push_back( track.LocationAtPoint(i) );
  }
  
  // otherwise flip order                                                                                                                                                                     
  else {
    for (size_t i=0; i < N; i++)
      sorted_trk.push_back( track.LocationAtPoint( N - i - 1) );
  }
}
/*************************** End Track Classification Functions Definitions  *************************************************************/


