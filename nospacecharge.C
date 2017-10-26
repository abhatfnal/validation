/*************************************************************
 *
 * demo_ReadHits() macro
 *
 * This is a simple demonstration of reading a LArSoft file
 * and accessing recob::Hit information.
 *
 * To run this, open root, and do:
 * root [0] .L demo_ReadHits.C++
 * root [1] demo_ReadHits()
 *
 * Wesley Ketchum (wketchum@fnal.gov), Oct31, 2016
 *
 *************************************************************/


//some standard C++ includes
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>

//some ROOT includes
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TChain.h"
//"art" includes (canvas, and gallery)
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"

//"larsoft" object includes
#include "lardataobj/RecoBase/Hit.h"

//convenient for us! let's not bother with art and std namespaces!
using namespace art;
using namespace std;

//I like doing this to not get fooled by underflow/overflow
void ShowUnderOverFlow( TH1* h1){
    h1->SetBinContent(1, h1->GetBinContent(0)+h1->GetBinContent(1));
    h1->SetBinContent(0,0);
    
    int nbins = h1->GetNbinsX();
    h1->SetBinContent(nbins, h1->GetBinContent(nbins)+h1->GetBinContent(nbins+1));
    h1->SetBinContent(nbins+1,0);
}

void nospacecharge() {
    
    //By default, Wes hates the stats box! But by default, Wes forgets to disable it in his ROOT profile stuff...
    gStyle->SetOptStat(0);
    
    //Let's make a histograms to store hit information!
  //  TH1F* h_track_hitsperchannel2 = new TH1F("h_hit_hitsperchannel2","Hits per channel;Channel Number;Number of Hits",8500,-0.5,8499.5);
    TH2D* h_track_start1 = new TH2D("h_track_start1","End Point of Tracks;X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
    h_track_start1->SetMarkerStyle(20);
    h_track_start1->SetMarkerColor(2);
        TH2D* h_track_start2 = new TH2D("h_track_start2","End Point of Tracks;X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
    h_track_start2->SetMarkerStyle(20);
    h_track_start2->SetMarkerColor(2);
    TH2D* h_track_start3 = new TH2D("h_track_start3","End Point of Tracks;X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
    h_track_start3->SetMarkerStyle(20);
    h_track_start3->SetMarkerColor(2);
    TH2D* h_track_start4 = new TH2D("h_track_start4","End Point of Tracks;X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
    h_track_start4->SetMarkerStyle(20);
    h_track_start4->SetMarkerColor(2);
    TH2D* h_track_start5 = new TH2D("h_track_start5","End Point of Tracks;X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
    h_track_start5->SetMarkerStyle(20);
    h_track_start5->SetMarkerColor(2);
    TH2D* h_track_start6 = new TH2D("h_track_start6","End Point of Tracks;X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
    h_track_start6->SetMarkerStyle(20);
    h_track_start6->SetMarkerColor(2);
    TH2D* h_track_start7 = new TH2D("h_track_start7","End Point of Tracks;X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
    h_track_start7->SetMarkerStyle(20);
    h_track_start7->SetMarkerColor(2);
    TH2D* h_track_start8 = new TH2D("h_track_start8","End Point of Tracks;X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
    h_track_start8->SetMarkerStyle(20);
    h_track_start8->SetMarkerColor(2);
    TH2D* h_track_start9 = new TH2D("h_track_start9","End Point of Tracks;X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
    h_track_start9->SetMarkerStyle(20);
    h_track_start9->SetMarkerColor(2);
    TH2D* h_track_start10 = new TH2D("h_track_start10","End Point of Tracks;X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
    h_track_start10->SetMarkerStyle(20);
    h_track_start10->SetMarkerColor(2);
    
    
    
    TH2D* h_track_start11 = new TH2D("h_track_start11","End Point of Tracks (nospacecharge);X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
    h_track_start11->SetMarkerStyle(20);
    h_track_start11->SetMarkerColor(4);
    TH2D* h_track_start12 = new TH2D("h_track_start12","End Point of Tracks (nospacecharge);X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
     h_track_start12->SetMarkerStyle(20);
    h_track_start12->SetMarkerColor(4);
    TH2D* h_track_start13 = new TH2D("h_track_start13","End Point of Tracks (nospacecharge);X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
     h_track_start13->SetMarkerStyle(20);
    h_track_start13->SetMarkerColor(4);
    TH2D* h_track_start14 = new TH2D("h_track_start14","End Point of Tracks (nospacecharge);X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
     h_track_start14->SetMarkerStyle(20);
    h_track_start14->SetMarkerColor(4);
    TH2D* h_track_start15 = new TH2D("h_track_start15","End Point of Tracks (nospacecharge);X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
     h_track_start15->SetMarkerStyle(20);
    h_track_start15->SetMarkerColor(4);
    TH2D* h_track_start16 = new TH2D("h_track_start16","End Point of Tracks (nospacecharge);X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
     h_track_start16->SetMarkerStyle(20);
    h_track_start16->SetMarkerColor(4);
    TH2D* h_track_start17 = new TH2D("h_track_start17","End Point of Tracks (nospacecharge);X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
     h_track_start17->SetMarkerStyle(20);
    h_track_start17->SetMarkerColor(4);
    TH2D* h_track_start18 = new TH2D("h_track_start18","End Point of Tracks (nospacecharge);X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
     h_track_start18->SetMarkerStyle(20);
    h_track_start18->SetMarkerColor(4);
    TH2D* h_track_start19 = new TH2D("h_track_start19","End Point of Tracks (nospacecharge);X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
     h_track_start19->SetMarkerStyle(20);
    h_track_start19->SetMarkerColor(4);
    TH2D* h_track_start20 = new TH2D("h_track_start20","End Point of Tracks (nospacecharge);X-Coordinate (cm);Y-Coordinate (cm)",100,-10,270,100,-130,130);
     h_track_start20->SetMarkerStyle(20);
    h_track_start20->SetMarkerColor(4);

    
    
    
    
    //We specify our files in a list of file names!
    //Note: multiple files allowed. Just separate by comma.
    vector<string> filename1 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363140_528/artroot_output.root" };
    vector<string> filename2 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363144_529/artroot_output.root" };
    vector<string> filename3 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363146_530/artroot_output.root" };
    vector<string> filename4 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363148_531/artroot_output.root" };
    vector<string> filename5 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363163_534/artroot_output.root" };
    vector<string> filename6 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363166_535/artroot_output.root" };
    vector<string> filename7 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363168_536/artroot_output.root" };
    vector<string> filename8 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363181_51/artroot_output.root" };
    vector<string> filename9 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363183_538/artroot_output.root" };
    vector<string> filename10 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363184_539/artroot_output.root" };
    
    vector<string> filename11 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/nospacecharge/19693778_528/artroot_output.root" };
    vector<string> filename12 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/nospacecharge/19693780_529/artroot_output.root" };
    vector<string> filename13 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/nospacecharge/19693782_530/artroot_output.root" };
    vector<string> filename14 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/nospacecharge/19693802_51/artroot_output.root" };
    vector<string> filename15 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/nospacecharge/19693788_532/artroot_output.root" };
    vector<string> filename16 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/nospacecharge/19693790_533/artroot_output.root" };
    vector<string> filename17 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/nospacecharge/19693794_535/artroot_output.root" };
    vector<string> filename18 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/nospacecharge/19693792_534/artroot_output.root" };
    vector<string> filename19 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/nospacecharge/19693796_536/artroot_output.root" };
    vector<string> filename20 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/nospacecharge/19693801_537/artroot_output.root" };
    

    
    
    
    
    
    
    
    
 //   vector<string> filename2 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/nospacecharge/19693222_33/artroot_output.root" };
    
    // std::vector< TChain* > chain;
    
    //  TChain* chain = new TChain("/uboone/app/users/abhat/larsoft_mydev/srcs/gallery_example/macros/outpoop.root");
    // TChain* chain = new TChain("mytree");
    // chain->Add("/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/*/artroot_output.root");
    
    //We need to specify the "input tag" for our collection of optical hits.
    //This is like the module label, except it can also include process name
    //and an instance label. Format is like this:
    //InputTag mytag{ "module_label","instance_label","process_name"};
    //You can ignore instance label if there isn't one. If multiple processes
    //used the same module label, the most recent one should be used by default.
    //
    //Check the contents of your file by setting up a version of larsoft, and
    //running an event dump:
    //  'lar -c eventdump.fcl -s MyInputFile_1.root -n 1 | grep "std::vector<recob::Hit>" '
    InputTag track_tag { "pandoraNu" };
    
    
    //ok, now for the event loop! Here's how it works.
    //
    //gallery has these built-in iterator things.
    //
    //You declare an event with a list of file names. Then, you
    //move to the next event by using the "next()" function.
    //Do that until you are "atEnd()"
    //In a for loop, that looks like this:
    // gallery::Event ev(chain);
    
    
    int counter1=0;
    for (gallery::Event ev(filename1); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter1++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
            
            for( auto const& track : track_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
             //   h_track_start1->Fill(track.End().X(),track.End().Y());
                
                h_track_start1->Fill(track.End().X(),track.End().Y());
                
               }
         // cout <<"There are "<< counter1<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    
    
    
    int counter2=0;
    for (gallery::Event ev(filename2); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter2++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
            
            
            for( auto const& track : track_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                //   h_track_start1->Fill(track.End().X(),track.End().Y());
                
                h_track_start2->Fill(track.End().X(),track.End().Y());
                
               }
         // cout <<"There are "<< counter2<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
   
    int counter3=0;
    for (gallery::Event ev(filename3); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter3++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
            
            for( auto const& track : track_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                //   h_track_start1->Fill(track.End().X(),track.End().Y());
                
                h_track_start3->Fill(track.End().X(),track.End().Y());
                
               }
        cout <<"There are "<<counter3<< " events in the file"<<endl;
        
        
    } //end loop over events!

    int counter4=0;
    for (gallery::Event ev(filename4); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter4++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
            
            for( auto const& track : track_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                //   h_track_start1->Fill(track.End().X(),track.End().Y());
                
                h_track_start4->Fill(track.End().X(),track.End().Y());
                
               }
     // cout <<"There are "<< counter4<< " events in the file"<<endl;
        
        
    } //end loop over events!

    
    
    int counter5=0;
    for (gallery::Event ev(filename5); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter5++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
            
            for( auto const& track : track_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                //   h_track_start1->Fill(track.End().X(),track.End().Y());
                
                h_track_start5->Fill(track.End().X(),track.End().Y());
                
               }
         // cout <<"There are "<< counter5<< " events in the file"<<endl;
        
        
    } //end loop over events!
 
    
    int counter6=0;
    for (gallery::Event ev(filename6); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter6++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
            
            for( auto const& track : track_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                //   h_track_start1->Fill(track.End().X(),track.End().Y());
                
                h_track_start6->Fill(track.End().X(),track.End().Y());
                
               }
         // cout <<"There are "<< counter6<< " events in the file"<<endl;
        
        
    } //end loop over events!

    
    int counter7=0;
    for (gallery::Event ev(filename7); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter7++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
            
            for( auto const& track : track_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                //   h_track_start1->Fill(track.End().X(),track.End().Y());
                
                h_track_start7->Fill(track.End().X(),track.End().Y());
                
               }
         // cout <<"There are "<< counter7<< " events in the file"<<endl;
        
        
    } //end loop over events!
 
    
    int counter8=0;
    for (gallery::Event ev(filename8); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter8++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
            
            for( auto const& track : track_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                //   h_track_start1->Fill(track.End().X(),track.End().Y());
                
                h_track_start8->Fill(track.End().X(),track.End().Y());
                
               }
         // cout <<"There are "<< counter8<< " events in the file"<<endl;
        
        
    } //end loop over events!

    
    
    int counter9=0;
    for (gallery::Event ev(filename9); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter9++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
            
            for( auto const& track : track_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                //   h_track_start1->Fill(track.End().X(),track.End().Y());
                
                h_track_start9->Fill(track.End().X(),track.End().Y());
                
               }
         // cout <<"There are "<< counter9<< " events in the file"<<endl;
        
        
    } //end loop over events!

    int counter10=0;
    for (gallery::Event ev(filename10); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter10++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
            
            for( auto const& track : track_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                //   h_track_start1->Fill(track.End().X(),track.End().Y());
                
                h_track_start10->Fill(track.End().X(),track.End().Y());
                
               }
         // cout <<"There are "<< counter10<< " events in the file"<<endl;
        
        
    } //end loop over events!
 
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    int counter11=0;
    for (gallery::Event ev(filename11); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter11++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
        
        for( auto const& track : track_vec){
            //    h_hit_integral->Fill(hit.Integral());
            // h_hit_peaktime->Fill(hit.PeakTime());
            // h_hit_peakamp->Fill(hit.PeakAmplitude());
            // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
            
            //   h_track_start1->Fill(track.End().X(),track.End().Y());
            
            h_track_start11->Fill(track.End().X(),track.End().Y());
            
          }
         // cout <<"There are "<< counter11<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    
    
    
    int counter12=0;
    for (gallery::Event ev(filename12); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter12++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
        
        
        for( auto const& track : track_vec){
            //    h_hit_integral->Fill(hit.Integral());
            // h_hit_peaktime->Fill(hit.PeakTime());
            // h_hit_peakamp->Fill(hit.PeakAmplitude());
            // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
            
            //   h_track_start1->Fill(track.End().X(),track.End().Y());
            
            h_track_start12->Fill(track.End().X(),track.End().Y());
            
          }
         // cout <<"There are "<< counter12<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    
    int counter13=0;
    for (gallery::Event ev(filename13); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter13++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
        
        for( auto const& track : track_vec){
            //    h_hit_integral->Fill(hit.Integral());
            // h_hit_peaktime->Fill(hit.PeakTime());
            // h_hit_peakamp->Fill(hit.PeakAmplitude());
            // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
            
            //   h_track_start1->Fill(track.End().X(),track.End().Y());
            
            h_track_start13->Fill(track.End().X(),track.End().Y());
            
          }
         // cout <<"There are "<< counter13<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    int counter14=0;
    for (gallery::Event ev(filename14); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter14++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
        
        for( auto const& track : track_vec){
            //    h_hit_integral->Fill(hit.Integral());
            // h_hit_peaktime->Fill(hit.PeakTime());
            // h_hit_peakamp->Fill(hit.PeakAmplitude());
            // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
            
            //   h_track_start1->Fill(track.End().X(),track.End().Y());
            
            h_track_start14->Fill(track.End().X(),track.End().Y());
            
          }
         // cout <<"There are "<< counter14<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    
    
    int counter15=0;
    for (gallery::Event ev(filename15); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter15++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
        
        for( auto const& track : track_vec){
            //    h_hit_integral->Fill(hit.Integral());
            // h_hit_peaktime->Fill(hit.PeakTime());
            // h_hit_peakamp->Fill(hit.PeakAmplitude());
            // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
            
            //   h_track_start1->Fill(track.End().X(),track.End().Y());
            
            h_track_start15->Fill(track.End().X(),track.End().Y());
            
          }
         // cout <<"There are "<< counter15<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    
    int counter16=0;
    for (gallery::Event ev(filename16); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter16++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
        
        for( auto const& track : track_vec){
            //    h_hit_integral->Fill(hit.Integral());
            // h_hit_peaktime->Fill(hit.PeakTime());
            // h_hit_peakamp->Fill(hit.PeakAmplitude());
            // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
            
            //   h_track_start1->Fill(track.End().X(),track.End().Y());
            
            h_track_start16->Fill(track.End().X(),track.End().Y());
            
          }
         // cout <<"There are "<< counter16<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    
    int counter17=0;
    for (gallery::Event ev(filename17); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter17++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
        
        for( auto const& track : track_vec){
            //    h_hit_integral->Fill(hit.Integral());
            // h_hit_peaktime->Fill(hit.PeakTime());
            // h_hit_peakamp->Fill(hit.PeakAmplitude());
            // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
            
            //   h_track_start1->Fill(track.End().X(),track.End().Y());
            
            h_track_start17->Fill(track.End().X(),track.End().Y());
            
          }
         // cout <<"There are "<< counter17<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    
    int counter18=0;
    for (gallery::Event ev(filename8); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter18++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
        
        for( auto const& track : track_vec){
            //    h_hit_integral->Fill(hit.Integral());
            // h_hit_peaktime->Fill(hit.PeakTime());
            // h_hit_peakamp->Fill(hit.PeakAmplitude());
            // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
            
            //   h_track_start1->Fill(track.End().X(),track.End().Y());
            
            h_track_start18->Fill(track.End().X(),track.End().Y());
            
          }
         // cout <<"There are "<< counter18<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    
    
    int counter19=0;
    for (gallery::Event ev(filename19); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter19++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
        
        for( auto const& track : track_vec){
            //    h_hit_integral->Fill(hit.Integral());
            // h_hit_peaktime->Fill(hit.PeakTime());
            // h_hit_peakamp->Fill(hit.PeakAmplitude());
            // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
            
            //   h_track_start1->Fill(track.End().X(),track.End().Y());
            
            h_track_start19->Fill(track.End().X(),track.End().Y());
            
          }
         // cout <<"There are "<< counter19<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    int counter20=0;
    for (gallery::Event ev(filename20); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& track_handle = ev.getValidHandle<vector<recob::Track>>(track_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& track_vec(*track_handle);
        
        counter20++;
        //  // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << track_vec.size() << " Tracks in this event." << endl;
        
        //We can fill our histogram for number of hits now!!!
        // h_hits_per_ev->Fill(hit_vec.size());
        
        //We can loop over the vector to get hit info too!
        //
        //Don't remember what's in a recob::Hit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/Hit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the hit integral/peak time/peak amplitude.
        //We can use a range-based for loop for ease.
        
        
        
        for( auto const& track : track_vec){
            //    h_hit_integral->Fill(hit.Integral());
            // h_hit_peaktime->Fill(hit.PeakTime());
            // h_hit_peakamp->Fill(hit.PeakAmplitude());
            // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
            
            //   h_track_start1->Fill(track.End().X(),track.End().Y());
            
            h_track_start20->Fill(track.End().X(),track.End().Y());
            
          }
         // cout <<"There are "<< counter20<< " events in the file"<<endl;
        
        
    } //end loop over events!

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //now, we're in a macro: we can just draw the histogram!
    //Let's make a TCanvas to draw our two histograms side-by-side
    TCanvas* canvas = new TCanvas("canvas","Hit Info!",1500,500);
//    canvas->Divide(2); //divides the canvas in three!
//    canvas->cd(1);     //moves us to the first canvas
    // h_hits_per_ev->Draw();
    // canvas->cd(2);     //moves us to the second canvas
    // ShowUnderOverFlow(h_hit_peaktime); //use this function to move under/overflow into visible bins.
    // h_hit_peaktime->Draw();
    // canvas->cd(3);     //moves us to the third canvas
    // ShowUnderOverFlow(h_hit_peakamp); //use this function to move under/overflow into visible bins.
    // ShowUnderOverFlow(h_hit_integral); //use this function to move under/overflow into visible bins.
    // h_hit_integral->SetLineColor(kRed);
    // h_hit_peakamp->SetLineColor(kBlue);
    // h_hit_peakamp->Draw();
    // h_hit_integral->Draw("same");
    // canvas-> cd(4);
    // h_hit_averagewidth->Draw();
    // canvas-> cd(5);
  //  h_track_start1->SetLineColor(kRed);
  //  h_track_start1->SetLineWidth(2);
  
//    h_hit_hitsperchannel2->SetLineColor(kBlue);
    
    h_track_start1->Draw("P");
    h_track_start2->Draw("P SAME");
    h_track_start3->Draw("P SAME");
    h_track_start4->Draw("P SAME");
    h_track_start5->Draw("P SAME");
    h_track_start6->Draw("P SAME");
    h_track_start7->Draw("P SAME");
    h_track_start8->Draw("P SAME");
    h_track_start9->Draw("P SAME");
    h_track_start10->Draw("P SAME");

    
 //   canvas->cd(2);
    h_track_start11->SetTitle("End Point of Tracks (nospacecharge)");
    h_track_start11->Draw("P SAME");
    h_track_start12->Draw("P SAME");
    h_track_start13->Draw("P SAME");
    h_track_start14->Draw("P SAME");
    h_track_start15->Draw("P SAME");
    h_track_start16->Draw("P SAME");
    h_track_start17->Draw("P SAME");
    h_track_start18->Draw("P SAME");
    h_track_start19->Draw("P SAME");
    h_track_start20->Draw("P SAME");
    
    TLine *l1 = new TLine(-10,-116.5,270,-116.5);
    TLine *l2 = new TLine(-10,116.5,270,116.5);
    l1->Draw();
    l2->Draw("SAME");
    /*
    
    h_hit_hitsperchannel2->Draw("SAME");
    h_hit_hitsperchannel3->SetLineColor(kGreen);
    h_hit_hitsperchannel3->Draw("SAME");
    h_hit_hitsperchannel4->SetLineColor(kOrange);
    h_hit_hitsperchannel4->Draw("SAME");
    // auto legend = new TLegend(0.1,0.9,0.48,0.9);
    // h_hit_hitsperchannel->SaveAs("combined.root");
    //and ... done!
     */
    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(h_track_start1,"CV","p");
    legend->AddEntry(h_track_start11,"nospacecharge","p");
    legend->Draw();
    
    
    }
