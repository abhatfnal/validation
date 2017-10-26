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

void dicharge(){
    
    
    //const int maxEvents = 1000000;
    const int maxEvents1 = 2;            //maxEvents just tells you how many events in the root file should be read to go to the desired event number
    const int maxEvents2 = 2;
    const int maxEvents3 = 1;
    const int maxEventsPerRun = 1000000;
    const int waveformEvent1 = 2;           //waveformEvent should match the maxEvents to be able to  read the desired event number
    const int waveformEvent2 = 2;
    const int waveformEvent3 = 1;
    
    const int maxTicks = 9594; // use with "daq" RawDigits     //maxTicks represents the x-axis range within which we wish to see the adc waveform
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
    
    vector<string> filename2 { "/pnfs/uboone/scratch/users/abhat/artroot_output_20171017T164934_detsim_20171017T172036_reco12.root" };
    
    vector<string> filename3 { "/pnfs/uboone/scratch/users/jaz8600/v06_26_01_06/detsyst/noPEnoise/22732203_162/artroot_output.root" };
    
    TH1F *WaveformHistCV_Chan3584 = new TH1F("WaveformHistCV_Chan3584","Channel 3584 Waveform;Time Tick;ADC Value",maxTicks,-0.5,maxTicks-0.5);
    
    WaveformHistCV_Chan3584->GetYaxis()->SetTitleOffset(1.2);
    
    TH1F *WaveformHistDIC_Chan3584 = new TH1F("WaveformHistDIC_Chan3584","Channel 3584 Waveform;Time Tick;ADC Value",maxTicks,-0.5,maxTicks-0.5);
    
    WaveformHistDIC_Chan3584->GetYaxis()->SetTitleOffset(1.2);
    
    TH1F *WaveformHistnoPEnoise_Chan3584 = new TH1F("WaveformHistnoPEnoise_Chan3584","Channel 3584 Waveform;Time Tick;ADC Value",maxTicks,-0.5,maxTicks-0.5);
    
    WaveformHistnoPEnoise_Chan3584->GetYaxis()->SetTitleOffset(1.2);

    
    
    
    int totalEventNum1 = 0;
    int totalEventNum2 = 0;
    int totalEventNum3 = 0;
    int prevRun1 = -99999;
    int prevRun2 = -99999;
    int prevRun3 = -99999;
    int runEventCount1 = 0;
    int runEventCount2 = 0;
    int runEventCount3 = 0;
    
    
    for (gallery::Event ev(filename1) ; !ev.atEnd(); ev.next()) {           //this for loop goes through all the events till you meet the criterion for the specific event you want
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
        if (totalEventNum1 > maxEvents1) {continue;}
        
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
                    
                    if (chanNum == 3584) {
                        WaveformHistCV_Chan3584->SetBinContent(k+1,ADCval);
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
        if (totalEventNum2 > maxEvents2) {continue;}
        
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
                    
                    if (chanNum2 == 3584) {
                        WaveformHistDIC_Chan3584->SetBinContent(k2+1,ADCval2);
                    }
                }
            }
        }
        
        auto t_end = high_resolution_clock::now();
        duration<double,std::milli> time_total_ms(t_end-t_begin);
        cout << "\tEvent took " << time_total_ms.count() << " ms to process." << endl;
    }
    
    
    
    
    
    
    
    for (gallery::Event ev(filename3) ; !ev.atEnd(); ev.next()) {
        
        
        auto t_begin = high_resolution_clock::now();
        
        int thisRunNum3 = ev.eventAuxiliary().run();
        int thisSubrunNum3 = ev.eventAuxiliary().subRun();
        int thisEventNum3 = ev.eventAuxiliary().event();
        
        if ((thisRunNum3 != prevRun3) && (runEventCount3 > 0)){
            prevRun3 = thisRunNum3;
            runEventCount3 = 1;
        }
        else if ((thisRunNum3 == prevRun3) && (runEventCount3 >= maxEventsPerRun)){
            continue;
        }
        else {
            prevRun3 = thisRunNum3;
            runEventCount3++;
        }
        
        totalEventNum3++;
        if (totalEventNum3 > maxEvents3) {continue;}
        
        cout << "Processing "
        << "Run " << thisRunNum3 << ", "
        << "Subrun " << thisSubrunNum3 << ", "
        << "Event " << thisEventNum3 << endl;
        
        auto const& rawdigit_handle = ev.getValidHandle<vector<raw::RawDigit>>(rawdigit_tag);
        auto const& allrawdigits_vec(*rawdigit_handle);
        
        for (size_t i_ar = 0, size_allrawdigits = rawdigit_handle->size(); i_ar != size_allrawdigits; ++i_ar) {
            int chanNum3 = allrawdigits_vec.at(i_ar).Channel();
            
            if (totalEventNum3 == waveformEvent3) {
                for (int k3 = 0; k3 < allrawdigits_vec.at(i_ar).Samples(); k3++) {
                    double ADCval3 = allrawdigits_vec.at(i_ar).ADC(k3);
                    
                    if (chanNum3 == 3584) {
                        WaveformHistnoPEnoise_Chan3584->SetBinContent(k3+1,ADCval3);
                    }
                }
            }
        }
        
        auto t_end = high_resolution_clock::now();
        duration<double,std::milli> time_total_ms(t_end-t_begin);
        cout << "\tEvent took " << time_total_ms.count() << " ms to process." << endl;
    }

    
    
    
    
    
    
    TH1* fftHistCV=0;
    TH1* fftHistDIC=0;
    
    
    
    TVirtualFFT::SetTransform(0);
    fftHistCV = WaveformHistCV_Chan3584->FFT(fftHistCV,"MAG");
    fftHistCV->SetNameTitle("fftHistCV", "FFT ;Time-ticks;FFT Magnitude");
    
    TVirtualFFT::SetTransform(0);
    fftHistDIC=WaveformHistDIC_Chan3584->FFT(fftHistDIC,"MAG");
    fftHistDIC->SetNameTitle("fftHistDIC", "FFT ;Time-ticks;FFT Magnitude");
    
    
    
    TH1F *WaveformHist_Chan3584Residue = (TH1F*) WaveformHistCV_Chan3584->Clone();
    WaveformHist_Chan3584Residue->SetNameTitle("WaveformHist_Chan3584Residue", "Waveform residue;Time-ticks;(CV - DIC)");
    WaveformHist_Chan3584Residue->Add(WaveformHistDIC_Chan3584,-1.0);
    
    TH1F *fftHistResidue = (TH1F*) fftHistCV->Clone();
    fftHistResidue->SetNameTitle("fftHistResidue", "FFT residue;Time-ticks;(CV - DIC)");
    fftHistResidue->Add(fftHistDIC,-1.0);
    
    
    TCanvas* canvas = new TCanvas("canvas","Waveform info!",1000,500);
 //   canvas->Divide(2); //divides the canvas in three!
 //   canvas->cd(1);     //moves us to the first half of canvas
    
    
    
    
    WaveformHistCV_Chan3584->SetLineColor(kRed);
    WaveformHistCV_Chan3584->SetLineWidth(2);
    WaveformHistCV_Chan3584->Draw();
    WaveformHistDIC_Chan3584->SetLineColor(kBlue);
    WaveformHistDIC_Chan3584->Draw("SAME");
    WaveformHistnoPEnoise_Chan3584->SetLineColor(kGreen);
 //   WaveformHistnoPEnoise_Chan3584->Draw("SAME");
    
    
    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->SetTextSize(0.04);
    legend->AddEntry(WaveformHistCV_Chan3584,"CV","l");
    legend->AddEntry(WaveformHistDIC_Chan3584,"DIC","l");
 // legend->AddEntry(WaveformHistnoPEnoise_Chan3584,"noPEnoise","l");

    legend->Draw();
  
     /*
    
    canvas->cd(2);
    
    WaveformHist_Chan3584Residue->SetLineColor(kBlue);
    WaveformHist_Chan3584Residue->Draw();
    
   
    
    canvas->cd(3);
    fftHistCV->SetBinContent(1,0);
    fftHistCV->SetLineColor(kRed);
    fftHistCV->SetLineWidth(2);
    fftHistCV->Draw("HIST");
    fftHistDIC->SetBinContent(1,0);
    fftHistDIC->SetLineColor(kBlue);
    fftHistDIC->Draw("SAME");
    legend->Draw();
    
    canvas->cd(4);
    fftHistResidue->SetBinContent(1,0);
    fftHistResidue->SetLineColor(kBlue);
    fftHistResidue->Draw();
     
    */
}
