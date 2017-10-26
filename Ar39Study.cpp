//some standard C++ includes
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <fstream>

//some ROOT includes
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TMath.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TCanvas.h"

//"art" includes (canvas, and gallery)
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"

//"larsoft" object includes
#include "lardataobj/RecoBase/OpFlash.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/AnalysisBase/T0.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h"

using namespace art;
using namespace std;
using namespace std::chrono;

void Ar39Study(){


  //const int maxEvents = 1000000;
  const int maxEvents = 1;
  const int maxEventsPerRun = 1000000;
  const int waveformEvent1 = 1;
    const int waveformEvent2 = 1;

  const int maxTicks = 9594; // use with "daq" RawDigits
  //const int maxTicks = 6400; // use with "wcNoiseFilter" RawDigits
    InputTag rawdigit_tag { "daq" };
  //InputTag rawdigit_tag { "wcNoiseFilter" };

  gStyle->SetOptStat(0); // gets rid of stat boxes in upper right of histograms

 /*
  vector<string> filename1;
  ifstream myfile("filelist_small.txt");
  copy(istream_iterator<string>(myfile),istream_iterator<string>(),back_inserter(filename1));

  TFile f_output("output.root","RECREATE");
  
  */
  
    
    vector<string> filename1 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/CV/19363140_528/artroot_output.root" };
    
    vector<string> filename2 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/whitenoise/22734236_277/artroot_output.root" };
  
    TH1F *WaveformHistCV_Chan230 = new TH1F("WaveformHistCV_Chan230","Channel 230 Waveform;Time Tick;ADC Value",maxTicks,-0.5,maxTicks-0.5);
  
    WaveformHistCV_Chan230->GetYaxis()->SetTitleOffset(1.2);

    TH1F *WaveformHistWhitenoise_Chan230 = new TH1F("WaveformHistWhitenoise_Chan230","Channel 230 Waveform;Time Tick;ADC Value",maxTicks,-0.5,maxTicks-0.5);
   
    WaveformHistWhitenoise_Chan230->GetYaxis()->SetTitleOffset(1.2);
    
    
    
  int totalEventNum1 = 0;
    int totalEventNum2 = 0;
  int prevRun1 = -99999;
    int prevRun2 = -99999;
  int runEventCount1 = 0;
    int runEventCount2 = 0;
  
 
    for (gallery::Event ev(filename1) ; !ev.atEnd(); ev.next()) {
    auto t_begin = high_resolution_clock::now();

    int thisRunNum = ev.eventAuxiliary().run();
    int thisSubrunNum = ev.eventAuxiliary().subRun();
    int thisEventNum = ev.eventAuxiliary().event();

    if ((thisRunNum != prevRun1) && (runEventCount1 > 0)){
      prevRun1 = thisRunNum;
      runEventCount1 = 1;
    }
    else if ((thisRunNum == prevRun1) && (runEventCount1 >= maxEventsPerRun)){
      continue;
    }
    else {
      prevRun1 = thisRunNum;
      runEventCount1++;
    }

    totalEventNum1++;
    if (totalEventNum1 > maxEvents) {continue;}

    cout << "Processing "
	 << "Run " << thisRunNum << ", "
	 << "Subrun " << thisSubrunNum << ", "
	 << "Event " << thisEventNum << endl;

    auto const& rawdigit_handle = ev.getValidHandle<vector<raw::RawDigit>>(rawdigit_tag);
    auto const& allrawdigits_vec(*rawdigit_handle);

    for (size_t i_ar = 0, size_allrawdigits = rawdigit_handle->size(); i_ar != size_allrawdigits; ++i_ar) {
      int chanNum = allrawdigits_vec.at(i_ar).Channel();

      if (totalEventNum1 == waveformEvent1) {
        for (int k = 0; k < allrawdigits_vec.at(i_ar).Samples(); k++) {
          double ADCval = allrawdigits_vec.at(i_ar).ADC(k);
      
          if (chanNum == 230) {
            WaveformHistCV_Chan230->SetBinContent(k+1,ADCval);
      	  }
        }
      }
    }

    auto t_end = high_resolution_clock::now();
    duration<double,std::milli> time_total_ms(t_end-t_begin);
    cout << "\tEvent took " << time_total_ms.count() << " ms to process." << endl;
    }
    
   

 
   
    
    
    
    
    
    
    
    

    
    for (gallery::Event ev(filename2) ; !ev.atEnd(); ev.next()) {
        
        
        auto t_begin = high_resolution_clock::now();
        
        int thisRunNum2 = ev.eventAuxiliary().run();
        int thisSubrunNum2 = ev.eventAuxiliary().subRun();
        int thisEventNum2 = ev.eventAuxiliary().event();
        
        if ((thisRunNum2 != prevRun2) && (runEventCount2 > 0)){
            prevRun2 = thisRunNum2;
            runEventCount2 = 1;
        }
        else if ((thisRunNum2 == prevRun2) && (runEventCount2 >= maxEventsPerRun)){
            continue;
        }
        else {
            prevRun2 = thisRunNum2;
            runEventCount2++;
        }
        
        totalEventNum2++;
        if (totalEventNum2 > maxEvents) {continue;}
        
        cout << "Processing "
        << "Run " << thisRunNum2 << ", "
        << "Subrun " << thisSubrunNum2 << ", "
        << "Event " << thisEventNum2 << endl;
        
        auto const& rawdigit_handle = ev.getValidHandle<vector<raw::RawDigit>>(rawdigit_tag);
        auto const& allrawdigits_vec(*rawdigit_handle);
        
        for (size_t i_ar = 0, size_allrawdigits = rawdigit_handle->size(); i_ar != size_allrawdigits; ++i_ar) {
            int chanNum2 = allrawdigits_vec.at(i_ar).Channel();
            
            if (totalEventNum2 == waveformEvent2) {
                for (int k2 = 0; k2 < allrawdigits_vec.at(i_ar).Samples(); k2++) {
                    double ADCval2 = allrawdigits_vec.at(i_ar).ADC(k2);
                    
                    if (chanNum2 == 230) {
                        WaveformHistWhitenoise_Chan230->SetBinContent(k2+1,ADCval2);
                    }
                }
            }
        }
        
        auto t_end = high_resolution_clock::now();
        duration<double,std::milli> time_total_ms(t_end-t_begin);
        cout << "\tEvent took " << time_total_ms.count() << " ms to process." << endl;
    }

 
 
    TH1* fftHistCV=0;
    TH1* fftHistWhitenoise=0;
    

    
    TVirtualFFT::SetTransform(0);
    fftHistCV = WaveformHistCV_Chan230->FFT(fftHistCV,"MAG");
    fftHistCV->SetNameTitle("fftHistCV", "FFT ;Frequency (MHz);Magnitude");
    
    TVirtualFFT::SetTransform(0);
    fftHistWhitenoise=WaveformHistWhitenoise_Chan230->FFT(fftHistWhitenoise,"MAG");
    fftHistWhitenoise->SetNameTitle("fftHistWhitenoise", "FFT ;Frequency (MHz);Magnitude");
    
    
    
    TH1F *WaveformHist_Chan230Residue = (TH1F*) WaveformHistCV_Chan230->Clone();
    WaveformHist_Chan230Residue->SetNameTitle("WaveformHist_Chan230Residue", "Waveform residue;Time-ticks;(CV - whitenoise)");
    WaveformHist_Chan230Residue->Add(WaveformHistWhitenoise_Chan230,-1.0);
    
    TH1F *fftHistResidue = (TH1F*) fftHistCV->Clone();
    fftHistResidue->SetNameTitle("fftHistResidue", "FFT Residue;Frequency (MHz);(CV - whitenoise)");
    fftHistResidue->Add(fftHistWhitenoise,-1.0);

    
    TCanvas* canvas = new TCanvas("canvas","Waveform info!",1000,500);
    canvas->Divide(2); //divides the canvas in three!
 /*
    canvas->cd(1);     //moves us to the first half of canvas

    
    
    
    WaveformHistCV_Chan230->SetLineColor(kRed);
    WaveformHistCV_Chan230->SetLineWidth(2);
    WaveformHistCV_Chan230->Draw();
    WaveformHistWhitenoise_Chan230->SetLineColor(kBlue);
    WaveformHistWhitenoise_Chan230->Draw("SAME");
    
    
    
   auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->SetTextSize(0.04);
    legend->AddEntry(WaveformHistCV_Chan230,"CV","l");
    legend->AddEntry(WaveformHistWhitenoise_Chan230,"Whitenoise","l");
    legend->Draw();
  
    canvas->cd(2);
    
    WaveformHist_Chan230Residue->SetLineColor(kBlue);
    WaveformHist_Chan230Residue->Draw();
*/
    canvas->cd(1);
    fftHistCV->SetBinContent(1,0);
    fftHistCV->SetLineColor(kRed);
    fftHistCV->SetLineWidth(2);
    fftHistCV->GetXaxis()->SetLimits(0.,2.);
    fftHistCV-> GetXaxis()->SetRangeUser(0.,1.);
    fftHistCV->Draw("HIST");
    fftHistWhitenoise->SetBinContent(1,0);
    fftHistWhitenoise->SetLineColor(kBlue);
    fftHistWhitenoise->GetXaxis()->SetLimits(0.,2.);
    fftHistWhitenoise-> GetXaxis()->SetRangeUser(0.,1.);
    fftHistWhitenoise->Draw("SAME");
    
    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->SetTextSize(0.04);
    legend->AddEntry(fftHistCV,"CV","l");
    legend->AddEntry(fftHistWhitenoise,"Whitenoise","l");
    legend->Draw();
    
    
    canvas->cd(2);
   fftHistResidue->SetBinContent(1,0);
    fftHistResidue->SetLineColor(kBlue);
    fftHistResidue-> GetXaxis()->SetLimits(0.,2.);
    fftHistResidue-> GetXaxis()->SetRangeUser(0.,1.);
    fftHistResidue->Draw();
 
 
}
