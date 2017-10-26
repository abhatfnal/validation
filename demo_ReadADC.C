/*************************************************************
 * 
 * demo_ReadADC() macro
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
#include "TBranch.h"
#include "TTree.h"
#include "TMath.h"

//"art" includes (canvas, and gallery)
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"

//"larsoft" object includes
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/OpFlash.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/AnalysisBase/T0.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h"


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

void demo_ReadADC() {

  //By default, Wes hates the stats box! But by default, Wes forgets to disable it in his ROOT profile stuff...
   gStyle->SetOptStat(0);

  //Let's make a histograms to store hit information!
   TH1F* h_rawdigit_ADCpertimetick2 = new TH1F("h_rawdigit_ADCpertimetick2","ADC per time tick;Time tick;ADC Value",10000,-0.5,9999.5);
   TH1F* h_rawdigit_ADCpertimetick1 = new TH1F("h_rawdigit_ADCpertimetick1","ADC per time tick;Time tick;ADC Value",10000,-0.5,9999.5);
   // TH1F* h_hit_hitsperchannel3 = new TH1F("h_hit_hitsperchannel3","Hits per channel;Channel Number;Number of Hits",8500,-0.5,8499.5);
 // TH1F* h_hit_hitsperchannel4 = new TH1F("h_hit_hitsperchannel4","Hits per channel;Channel Number;Number of Hits",8500,-0.5,8499.5);
  // TH1F* h_hit_hitsperchannel5 = new TH1F("h_hit_hitsperchannel5","Hits per channel;Channel Number;Number of Hits",8500,-0.5,8499.5);

   //We specify our files in a list of file names!
  //Note: multiple files allowed. Just separate by comma.
  vector<string> filename1 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363140_528/artroot_output.root" }; 
 vector<string> filename2 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/whitenoise/22734236_277/artroot_output.root" };
//vector<string> filename3 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/noPEnoise/22732203_162/artroot_output.root" };
//vector<string> filename4 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/lowgain/19364867_633/artroot_output.root" };
//vector<string> filename5 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/altDeadChannels/22731397_52/artroot_output.root" };
    
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
  InputTag rawdigit_tag { "daq" };


  //ok, now for the event loop! Here's how it works.
  //
  //gallery has these built-in iterator things.
  //
  //You declare an event with a list of file names. Then, you
  //move to the next event by using the "next()" function.
  //Do that until you are "atEnd()"
  //In a for loop, that looks like this:
  // gallery::Event ev(chain);
  

  int counter=0;
  for (gallery::Event ev(filename1); !ev.atEnd(); ev.next()) {

    //to get run and event info, you use this "eventAuxillary()" object.
    cout << "Processing "
	 << "Run " << ev.eventAuxiliary().run() << ", "
	 << "Event " << ev.eventAuxiliary().event() << endl;
    
    //Now, we want to get a "valid handle" (which is like a pointer to our collection")
    //We use auto, cause it's annoying to write out the fill type. But it's like
    //vector<recob::Hit>* object.
    auto const& rawdigit_handle = ev.getValidHandle<vector<raw::RawDigit>>(rawdigit_tag);

    //We can now treat this like a pointer, or dereference it to have it be like a vector.
    //I (Wes) for some reason prefer the latter, so I always like to do ...
    auto const& rawdigit_vec(*rawdigit_handle);

    counter++;
    // cout <<"There are "<< counter<< "events in the file"<<endl;
    cout << "\tThere are " << rawdigit_vec.size() << " Hits in this event." << endl;
    
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

    if (ev.eventAuxiliary().event()==311377){


    for( auto const& rawdigit : rawdigit_vec){
    //    h_hit_integral->Fill(hit.Integral());
    // h_hit_peaktime->Fill(hit.PeakTime());
    // h_hit_peakamp->Fill(hit.PeakAmplitude());
    // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());

      h_rawdigit_ADCpertimetick1->Fill(rawdigit.ADC(1460));
    }}
    cout <<"There are "<< counter<< " events in the file"<<endl;

    
  } //end loop over events!


 

   int counterr=0;
 for (gallery::Event ev(filename2); !ev.atEnd(); ev.next()) {
 
 //to get run and event info, you use this "eventAuxillary()" object.
 cout << "Processing "
 << "Run " << ev.eventAuxiliary().run() << ", "
 << "Event " << ev.eventAuxiliary().event() << endl;
 
 //Now, we want to get a "valid handle" (which is like a pointer to our collection")
 //We use auto, cause it's annoying to write out the fill type. But it's like
 //vector<recob::Hit>* object.
 auto const& rawdigit_handle = ev.getValidHandle<vector<raw::RawDigit>>(rawdigit_tag);
 
 //We can now treat this like a pointer, or dereference it to have it be like a vector.
 //I (Wes) for some reason prefer the latter, so I always like to do ...
 auto const& rawdigit_vec(*rawdigit_handle);
 
 counterr++;
 // cout <<"There are "<< counter<< "events in the file"<<endl;
 cout << "\tThere are " << rawdigit_vec.size() << " Hits in this event." << endl;
 
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
 
 if (ev.eventAuxiliary().event()==311377){
 
 
 for( auto const& rawdigit : rawdigit_vec){
 //    h_hit_integral->Fill(hit.Integral());
 // h_hit_peaktime->Fill(hit.PeakTime());
 // h_hit_peakamp->Fill(hit.PeakAmplitude());
 // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
 
 h_rawdigit_ADCpertimetick2->Fill(rawdigit.ADC(1460));
 }}
 cout <<"There are "<< counterr<< " events in the file"<<endl;
 
 
 } //end loop over events!
 
/*
    int counterrr=0;
    for (gallery::Event ev(filename3); !ev.atEnd(); ev.next()) {
 
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& hit_handle = ev.getValidHandle<vector<recob::Hit>>(hit_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& hit_vec(*hit_handle);
        
        counterrr++;
        // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << hit_vec.size() << " Hits in this event." << endl;
        
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
        
        if (ev.eventAuxiliary().event()==311377){
            
            
            for( auto const& hit : hit_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                h_hit_hitsperchannel3->Fill(hit.Channel());
            }}
        cout <<"There are "<< counterrr<< " events in the file"<<endl;
        
        
    } //end loop over events!


    int counterrrr=0;
    for (gallery::Event ev(filename4); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& hit_handle = ev.getValidHandle<vector<recob::Hit>>(hit_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& hit_vec(*hit_handle);
        
        counterrrr++;
        // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << hit_vec.size() << " Hits in this event." << endl;
        
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
        
        if (ev.eventAuxiliary().event()==311377){
            
            
            for( auto const& hit : hit_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                h_hit_hitsperchannel4->Fill(hit.Channel());
            }}
        cout <<"There are "<< counterrrr<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    int counterrrrr=0;
    for (gallery::Event ev(filename5); !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::Hit>* object.
        auto const& hit_handle = ev.getValidHandle<vector<recob::Hit>>(hit_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& hit_vec(*hit_handle);
        
        counterrrrr++;
        // cout <<"There are "<< counter<< "events in the file"<<endl;
        cout << "\tThere are " << hit_vec.size() << " Hits in this event." << endl;
        
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
        
        if (ev.eventAuxiliary().event()==311377){
            
            
            for( auto const& hit : hit_vec){
                //    h_hit_integral->Fill(hit.Integral());
                // h_hit_peaktime->Fill(hit.PeakTime());
                // h_hit_peakamp->Fill(hit.PeakAmplitude());
                // h_hit_averagewidth->Fill(hit.EndTick () - hit.StartTick());
                
                h_hit_hitsperchannel5->Fill(hit.Channel());
            }}
        cout <<"There are "<< counterrrrr<< " events in the file"<<endl;
        
        
    } //end loop over events!
    
    
    
    
    
    
    
    
    
    
 
    
    
  TH1F *h_hit_residue1 = new TH1F(*h_hit_hitsperchannel1);
  h_hit_residue1->SetNameTitle("h_hit_residue", "Hits per channel residue;Channel Number;(CV - Syst)");
  h_hit_residue1->Add(h_hit_hitsperchannel2,-1.0);

    TH1F *h_hit_residue2 = new TH1F(*h_hit_hitsperchannel1);
    h_hit_residue2->SetNameTitle("h_hit_residue", "Hits per channel residue;Channel Number;(CV - Syst)");
    h_hit_residue2->Add(h_hit_hitsperchannel3,-1.0);
  
    TH1F *h_hit_residue3 = new TH1F(*h_hit_hitsperchannel1);
    h_hit_residue3->SetNameTitle("h_hit_residue", "Hits per channel residue;Channel Number;(CV - Syst)");
    h_hit_residue3->Add(h_hit_hitsperchannel4,-1.0);

    TH1F *h_hit_residue4 = new TH1F(*h_hit_hitsperchannel1);
    h_hit_residue4->SetNameTitle("h_hit_residue", "Hits per channel residue;Channel Number;(CV - Syst)");
    h_hit_residue4->Add(h_hit_hitsperchannel5,-1.0);

  */
  //now, we're in a macro: we can just draw the histogram!
  //Let's make a TCanvas to draw our two histograms side-by-side
  TCanvas* canvas = new TCanvas("canvas","Hit Info!",1500,500);
  //canvas->Divide(2); //divides the canvas in three!
 // canvas->cd(1);     //moves us to the first canvas
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
   h_rawdigit_ADCpertimetick1->SetLineColor(kRed);
   h_rawdigit_ADCpertimetick1->SetLineWidth(2);
   h_rawdigit_ADCpertimetick2->SetLineColor(kBlue);
   h_rawdigit_ADCpertimetick1->Draw();
   h_rawdigit_ADCpertimetick2->Draw("SAME");
  /* h_hit_hitsperchannel2->Draw("SAME");
   h_hit_hitsperchannel3->SetLineColor(kGreen);
   h_hit_hitsperchannel3->Draw("SAME");
    h_hit_hitsperchannel4->SetLineColor(kOrange);
    h_hit_hitsperchannel4->Draw("SAME");
    h_hit_hitsperchannel5->SetLineColor(kCyan);
    h_hit_hitsperchannel5->Draw("SAME");
  // auto legend = new TLegend(0.1,0.9,0.48,0.9);
   // h_hit_hitsperchannel->SaveAs("combined.root");
  //and ... done!

  auto legend = new TLegend(0.1,0.7,0.48,0.9);
   legend->AddEntry(h_hit_hitsperchannel1,"CV","l");
   legend->AddEntry(h_hit_hitsperchannel2,"deadSaturatedChannels","l");
   legend->AddEntry(h_hit_hitsperchannel3,"noPEnoise","l");
    legend->AddEntry(h_hit_hitsperchannel4,"lowgain","l");
    legend->AddEntry(h_hit_hitsperchannel5,"altDeadChannels","l");
    legend->Draw();

   canvas->cd(2);
    h_hit_residue1->SetLineColor(kBlue);
    h_hit_residue2->SetLineColor(kGreen);
    h_hit_residue3->SetLineColor(kOrange);
    h_hit_residue4->SetLineColor(kCyan);
    h_hit_residue1->Draw();
    h_hit_residue2->Draw("SAME");
    h_hit_residue3->Draw("SAME");
    h_hit_residue4->Draw("SAME");
    legend->Draw();
   
  */
}
