/*************************************************************
 * 
 * demo_ReadOpHits() macro
 * 
 * This is a simple demonstration of reading a LArSoft file 
 * and accessing recob::OpHit information.
 *
 * Wesley Ketchum (wketchum@fnal.gov), Aug28, 2016
 * 
 *************************************************************/


//some standard C++ includes
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

//some ROOT includes
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TFile.h"

//"art" includes (canvas, and gallery)
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"

//"larsoft" object includes
#include "lardataobj/RecoBase/OpHit.h"

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

void demo_ReadOpHits() {

  //By default, Wes hates the stats box! But by default, Wes forgets to disable it in his ROOT profile stuff...
  gStyle->SetOptStat(0);

  //Let's make a histograms to store optical hit information!
  // TH1F* h_ophits_per_ev = new TH1F("h_ophits_per_ev","OpHits per event;N_{optical hits};Events / bin",100,0,1000); 
  TH1F* h_ophit_pe1 = new TH1F("h_ophit_pe1","OpHit PEs; PE; Events / 0.1 PE",10000,0,1000);
  TH1F* h_ophit_pe2 = new TH1F("h_ophit_pe2","OpHit PEs; PE; Events / 0.1 PE",10000,0,1000);
      TH1F* h_ophit_pe3 = new TH1F("h_ophit_pe3","OpHit PEs; PE; Events / 0.1 PE",10000,0,1000);
    TH1F* h_ophit_pe4 = new TH1F("h_ophit_pe4","OpHit PEs; PE; Events / 0.1 PE",10000,0,1000);
    TH1F* h_ophit_pe5 = new TH1F("h_ophit_pe5","OpHit PEs; PE; Events / 0.1 PE",10000,0,1000);
  // TH1F* h_ophit_time = new TH1F("h_ophit_time","OpHit Time; t (#mus); Events / 1 #mus",200,-100,100);
  TH1F* h_ophits_per_channel1 = new TH1F("h_ophits_per_channel1","OpHits per channel;Channel Number;Number of OpHits",35,-0.5,34.5);
  TH1F* h_ophits_per_channel2 = new TH1F("h_ophits_per_channel2","OpHits per channel;Channel Number;Number of OpHits",35,-0.5,34.5);
  TH1F* h_ophits_per_channel3 = new TH1F("h_ophits_per_channel3","OpHits per channel;Channel Number;Number of OpHits",35,-0.5,34.5);
    TH1F* h_ophits_per_channel4 = new TH1F("h_ophits_per_channel4","OpHits per channel;Channel Number;Number of OpHits",35,-0.5,34.5);
    TH1F* h_ophits_per_channel5 = new TH1F("h_ophits_per_channel5","OpHits per channel;Channel Number;Number of OpHits",35,-0.5,34.5);
    
    // TH1F* h_ophits_per_channel2 = new THIF("h_ophits_per_channel2","OpHits per channel;Channel Number;Number of OpHits",8500,-0.5,8499.5);
  //We specify our files in a list of file names!
  //Note: multiple files allowed. Just separate by comma.
  vector<string> filename1 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363140_528/artroot_output.root" };
  vector<string> filename2 {"/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/noPEnoise/22732203_162/artroot_output.root"};
    vector<string> filename3 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/deadSaturatedChannels/19363426_203/artroot_output.root"};
vector<string> filename4 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/lowgain/19364867_633/artroot_output.root" };
    vector<string> filename5 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/altDeadChannels/22731397_52/artroot_output.root" };
  //We need to specify the "input tag" for our collection of optical hits.
  //This is like the module label, except it can also include process name
  //and an instance label. Format is like this:
  //InputTag mytag{ "module_label","instance_label","process_name"};
  //You can ignore instance label if there isn't one. If multiple processes
  //used the same module label, the most recent one should be used by default.
  //
  //Check the contents of your file by setting up a version of uboonecode, and
  //running an event dump:
  //  'lar -c eventdump.fcl -s MyInputFile_1.root -n 1 | grep ophit '
  InputTag ophit_tag { "ophitBeam" };


  //ok, now for the event loop! Here's how it works.
  //
  //gallery has these built-in iterator things.
  //
  //You declare an event with a list of file names. Then, you
  //move to the next event by using the "next()" function.
  //Do that until you are "atEnd()".
  //
  //In a for loop, that looks like this:
  int counter=0;
  for (gallery::Event ev(filename1) ; !ev.atEnd(); ev.next()) {

    //to get run and event info, you use this "eventAuxillary()" object.
    cout << "Processing "
	 << "Run " << ev.eventAuxiliary().run() << ", "
	 << "Event " << ev.eventAuxiliary().event() << endl;

    //Now, we want to get a "valid handle" (which is like a pointer to our collection")
    //We use auto, cause it's annoying to write out the fill type. But it's like
    //vector<recob::OpHit>* object.
    auto const& ophit_handle = ev.getValidHandle<vector<recob::OpHit>>(ophit_tag);

    //We can now treat this like a pointer, or dereference it to have it be like a vector.
    //I (Wes) for some reason prefer the latter, so I always like to do ...
    auto const& ophit_vec(*ophit_handle);
    counter++;
    //For good measure, print out the number of optical hits
    cout << "\tThere are " << ophit_vec.size() << " OpHits in this event." << endl;
    
    //We can fill our histogram for number of op hits now!!!
    // h_ophits_per_ev->Fill(ophit_vec.size());

    //We can loop over the vector to get optical hit info too!
    //
    //Don't remember what's in a recob::OpHit? Me neither. So, we can look it up.
    //The environment variable $LARDATAOBJ_DIR contains the directory of the
    //lardataobj product we set up. Inside that directory we can find what we need:
    // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/OpHit.h '
    //Note: it's the same directory structure as the include, after you go into
    //the 'source' directory. Look at that file and see what you can access.
    //
    //So, let's fill our histogram for the ophit PEs.
    //We can use a range-based for loop for ease.
      
      if (ev.eventAuxiliary().event()==311377){
    for( auto const& ophit : ophit_vec){
      h_ophit_pe1->Fill(ophit.PE());
     
      // h_ophit_time->Fill(ophit.PeakTime());
      h_ophits_per_channel1-> Fill(ophit.OpChannel());

    }}

    cout <<"There are "<< counter<< " events in the file"<<endl;
    
  } //end loop over events!


  int counterr=0;
  for (gallery::Event ev(filename2) ; !ev.atEnd(); ev.next()) {
    
    //to get run and event info, you use this "eventAuxillary()" object.                                                                                          
    cout << "Processing "
         << "Run " << ev.eventAuxiliary().run() << ", "
         << "Event " << ev.eventAuxiliary().event() << endl;
    
    //Now, we want to get a "valid handle" (which is like a pointer to our collection")                                                                           
    //We use auto, cause it's annoying to write out the fill type. But it's like                                                                                  
    //vector<recob::OpHit>* object.                                                                                                                               
    auto const& ophit_handle = ev.getValidHandle<vector<recob::OpHit>>(ophit_tag);
    
    //We can now treat this like a pointer, or dereference it to have it be like a vector.                                                                        
    //I (Wes) for some reason prefer the latter, so I always like to do ...                                                                                       
    auto const& ophit_vec(*ophit_handle);
    counterr++;
    //For good measure, print out the number of optical hits                                                                                                      
    cout << "\tThere are " << ophit_vec.size() << " OpHits in this event." << endl;
    
    //We can fill our histogram for number of op hits now!!!                                                                                                      
    // h_ophits_per_ev->Fill(ophit_vec.size());                                                                                                                   
    
    //We can loop over the vector to get optical hit info too!                                                                                                    
    //                                                                                                                                                            
    //Don't remember what's in a recob::OpHit? Me neither. So, we can look it up.                                                                                 
    //The environment variable $LARDATAOBJ_DIR contains the directory of the                                                                                      
    //lardataobj product we set up. Inside that directory we can find what we need:                                                                               
    // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/OpHit.h '                                                                                                  
    //Note: it's the same directory structure as the include, after you go into                                                                                   
    //the 'source' directory. Look at that file and see what you can access.                                                                                      
    //                                                                                                                                                            
    //So, let's fill our histogram for the ophit PEs.                                                                                                             
    //We can use a range-based for loop for ease.
      
      
      if (ev.eventAuxiliary().event()==311377){
    for( auto const& ophit : ophit_vec){
      h_ophit_pe2->Fill(ophit.PE());  
     
      // h_ophit_time->Fill(ophit.PeakTime());                                                                                                                    
      h_ophits_per_channel2-> Fill(ophit.OpChannel());
      
    }}
    
    
    
    
    cout <<"There are "<< counterr<< " events in the file"<<endl;
    
  } //end loop over events!                                                                                                                                       
  
  
    int counterrr=0;
    for (gallery::Event ev(filename3) ; !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::OpHit>* object.
        auto const& ophit_handle = ev.getValidHandle<vector<recob::OpHit>>(ophit_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& ophit_vec(*ophit_handle);
        counterrr++;
        //For good measure, print out the number of optical hits
        cout << "\tThere are " << ophit_vec.size() << " OpHits in this event." << endl;
        
        //We can fill our histogram for number of op hits now!!!
        // h_ophits_per_ev->Fill(ophit_vec.size());
        
        //We can loop over the vector to get optical hit info too!
        //
        //Don't remember what's in a recob::OpHit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/OpHit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the ophit PEs.
        //We can use a range-based for loop for ease.
        
        
        if (ev.eventAuxiliary().event()==311377){
            for( auto const& ophit : ophit_vec){
                h_ophit_pe3->Fill(ophit.PE());
                
                // h_ophit_time->Fill(ophit.PeakTime());
                h_ophits_per_channel3-> Fill(ophit.OpChannel());
                
            }}
        
        
        
        
        cout <<"There are "<< counterrr<< " events in the file"<<endl;
        
    } //end loop over events!
    
    int counterrrr=0;
    for (gallery::Event ev(filename4) ; !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::OpHit>* object.
        auto const& ophit_handle = ev.getValidHandle<vector<recob::OpHit>>(ophit_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& ophit_vec(*ophit_handle);
        counterrrr++;
        //For good measure, print out the number of optical hits
        cout << "\tThere are " << ophit_vec.size() << " OpHits in this event." << endl;
        
        //We can fill our histogram for number of op hits now!!!
        // h_ophits_per_ev->Fill(ophit_vec.size());
        
        //We can loop over the vector to get optical hit info too!
        //
        //Don't remember what's in a recob::OpHit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/OpHit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the ophit PEs.
        //We can use a range-based for loop for ease.
        
        
        if (ev.eventAuxiliary().event()==311377){
            for( auto const& ophit : ophit_vec){
                h_ophit_pe4->Fill(ophit.PE());
                
                // h_ophit_time->Fill(ophit.PeakTime());
                h_ophits_per_channel4-> Fill(ophit.OpChannel());
                
            }}
        
        
        
        
        cout <<"There are "<< counterrrr<< " events in the file"<<endl;
        
    } //end loop over events!
    
    
    
    int counterrrrr=0;
    for (gallery::Event ev(filename5) ; !ev.atEnd(); ev.next()) {
        
        //to get run and event info, you use this "eventAuxillary()" object.
        cout << "Processing "
        << "Run " << ev.eventAuxiliary().run() << ", "
        << "Event " << ev.eventAuxiliary().event() << endl;
        
        //Now, we want to get a "valid handle" (which is like a pointer to our collection")
        //We use auto, cause it's annoying to write out the fill type. But it's like
        //vector<recob::OpHit>* object.
        auto const& ophit_handle = ev.getValidHandle<vector<recob::OpHit>>(ophit_tag);
        
        //We can now treat this like a pointer, or dereference it to have it be like a vector.
        //I (Wes) for some reason prefer the latter, so I always like to do ...
        auto const& ophit_vec(*ophit_handle);
        counterrrrr++;
        //For good measure, print out the number of optical hits
        cout << "\tThere are " << ophit_vec.size() << " OpHits in this event." << endl;
        
        //We can fill our histogram for number of op hits now!!!
        // h_ophits_per_ev->Fill(ophit_vec.size());
        
        //We can loop over the vector to get optical hit info too!
        //
        //Don't remember what's in a recob::OpHit? Me neither. So, we can look it up.
        //The environment variable $LARDATAOBJ_DIR contains the directory of the
        //lardataobj product we set up. Inside that directory we can find what we need:
        // ' ls $LARDATAOBJ_DIR/source/lardataobj/RecoBase/OpHit.h '
        //Note: it's the same directory structure as the include, after you go into
        //the 'source' directory. Look at that file and see what you can access.
        //
        //So, let's fill our histogram for the ophit PEs.
        //We can use a range-based for loop for ease.
        
        
        if (ev.eventAuxiliary().event()==311377){
            for( auto const& ophit : ophit_vec){
                h_ophit_pe5->Fill(ophit.PE());
                
                // h_ophit_time->Fill(ophit.PeakTime());
                h_ophits_per_channel5-> Fill(ophit.OpChannel());
                
            }}
        
        
        
        
        cout <<"There are "<< counterrrrr<< " events in the file"<<endl;
        
    } //end loop over events!

    
    TH1F *h_ophit_residue1 = new TH1F(*h_ophits_per_channel1);
    h_ophit_residue1->SetNameTitle("h_hit_residue", "OpHits per channel residue;Channel Number;(CV - Syst)");
    h_ophit_residue1->Add(h_ophits_per_channel2,-1.0);
    
    TH1F *h_ophit_residue2 = new TH1F(*h_ophits_per_channel1);
    h_ophit_residue2->SetNameTitle("h_hit_residue", "OpHits per channel residue;Channel Number;(CV - Syst)");
    h_ophit_residue2->Add(h_ophits_per_channel3,-1.0);
    
    TH1F *h_ophit_residue3 = new TH1F(*h_ophits_per_channel1);
    h_ophit_residue3->SetNameTitle("h_hit_residue", "OpHits per channel residue;Channel Number;(CV - Syst)");
    h_ophit_residue3->Add(h_ophits_per_channel4,-1.0);
    
    TH1F *h_ophit_residue4 = new TH1F(*h_ophits_per_channel1);
    h_ophit_residue4->SetNameTitle("h_hit_residue", "OpHits per channel residue;Channel Number;(CV - Syst)");
    h_ophit_residue4->Add(h_ophits_per_channel5,-1.0);
    

    
    
    
    
    
    
  //now, we're in a macro: we can just draw the histogram!
  //Let's make a TCanvas to draw our two histograms side-by-side
  TCanvas* canvas = new TCanvas("canvas","OpHit Info!",1000,500);
  canvas->Divide(2); //divides the canvas in three!
   canvas->cd(1);     //moves us to the first half of canvas
  // h_ophits_per_ev->Draw();
  //  canvas->cd(2);     //moves us to the second half
  // ShowUnderOverFlow(h_ophit_pe); //use this function to move under/overflow into visible bins. 
  // h_ophit_pe->Draw();

   h_ophits_per_channel1->SetLineColor(kRed);
   h_ophits_per_channel1->SetLineWidth(2);
   h_ophits_per_channel2->SetLineColor(kGreen);
   h_ophits_per_channel3->SetLineColor(kBlue);
   h_ophits_per_channel4->SetLineColor(kOrange);
   h_ophits_per_channel5->SetLineColor(kCyan);
   h_ophits_per_channel1->Draw();
   h_ophits_per_channel2-> Draw("SAME");
   //  h_ophits_per_channel3-> Draw("SAME");
   // h_ophits_per_channel4-> Draw("SAME");
   // h_ophits_per_channel5-> Draw("SAME");
    
    
    
    
   auto legend = new TLegend(0.1,0.7,0.48,0.9);
    
    legend->SetTextSize(0.03);
  legend->AddEntry(h_ophits_per_channel1,"CV","l");
    
  legend->AddEntry(h_ophits_per_channel2,"noPEnoise","l");
  //  legend->AddEntry(h_ophits_per_channel3,"deadSaturatedChannels","l");
  // legend->AddEntry(h_ophits_per_channel4,"lowgain","l");
  // legend->AddEntry(h_ophits_per_channel5,"altDeadChannels","l");
  legend->Draw();



  /*  
 canvas->cd(2);     //moves us to the second half 
    gPad->SetLogy();
    h_ophit_pe1->SetLineColor(kRed);
    h_ophit_pe2->SetLineColor(kGreen);
    h_ophit_pe3->SetLineColor(kBlue);
    h_ophit_pe4->SetLineColor(kOrange);
    h_ophit_pe5->SetLineColor(kCyan);
    h_ophit_pe1->Draw();
    h_ophit_pe2->Draw("SAME");
    h_ophit_pe3->Draw("SAME");
    h_ophit_pe4->Draw("SAME");
    h_ophit_pe5->Draw("SAME");
    
    legend->Draw();
  */

 
  
  canvas->cd(2);     //moves us to the second half

    
    h_ophit_residue1->SetLineColor(kGreen);
    h_ophit_residue2->SetLineColor(kBlue);
     h_ophit_residue3->SetLineColor(kOrange);
    h_ophit_residue4->SetLineColor(kCyan);
    h_ophit_residue1->Draw();
    //   h_ophit_residue2-> Draw("SAME");
    //  h_ophit_residue3-> Draw("SAME");
    //  h_ophit_residue4-> Draw("SAME");
    legend->Draw();
  //and ... done!

    /*
    canvas->cd(4);     //moves us to the second half
    
    h_ophit_pe1->SetLineColor(kRed);
    h_ophit_pe2->SetLineColor(kGreen);
    h_ophit_pe3->SetLineColor(kBlue);
    h_ophit_pe4->SetLineColor(kOrange);
    h_ophit_pe5->SetLineColor(kCyan);
    h_ophit_pe1->Draw();
    h_ophit_pe2->Draw("SAME");
    h_ophit_pe3->Draw("SAME");
    h_ophit_pe4->Draw("SAME");
    h_ophit_pe5->Draw("SAME");
    
    
    legend->Draw();

    */



}
