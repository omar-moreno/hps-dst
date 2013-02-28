
void readHpsTree(string root_file)
{
    TFile *file = new TFile(root_file.c_str()); 
    TTree *tree = (TTree*) file->Get("HPS"); 

    vector<double> *px;
    vector<double> *py;
    vector<double> *pz; 
    int n_tracks; 
    double p; 

    tree->SetBranchAddress("px", &px); 
    tree->SetBranchAddress("py", &py); 
    tree->SetBranchAddress("pz", &pz); 
    tree->SetBranchAddress("n_tracks", &n_tracks); 

    TH1F *single_trk_p = new TH1F("single_trk_p", "Single Track Momentum", 10, 0, 6);
    single_trk_p->SetStats(0); 
    single_trk_p->SetLineColor(kBlue-7);
    single_trk_p->SetLineWidth(2);

    for(int entry = 0; entry < tree->GetEntries(); ++entry){
        tree->GetEntry(entry); 

        if(n_tracks != 1) continue;  

        p = sqrt(px->at(0)*px->at(0) + py->at(0)*py->at(0) + pz->at(0)*pz->at(0));        
        single_trk_p->Fill(p);  
    }

    single_trk_p->GetXaxis()->SetTitle("Track Momentum [Gev]");
    single_trk_p->GetXaxis()->SetTitleSize(0.03);
    single_trk_p->GetXaxis()->SetLabelSize(0.03);
    single_trk_p->Draw(""); 


}
