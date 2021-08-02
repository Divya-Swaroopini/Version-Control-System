/*
void product::search(int addr, char k[])
{
        int found=0,i;
        char dummy[10],a[10];
        i=addr;
        file.open(shopfile,ios::in|ios::out);
        do
        {
                file.seekg(i*recsize,ios::beg);
                file.getline(dummy,5,'\n');
                if(strcmp(dummy,"####")==0)
                        break;
                file.seekg(i*recsize,ios::beg);
                file.getline(key,15,'|');
                if(strcmp(key,k)==0)
                {
                        found=1;

                        file.getline(pname,20,'|');
                        file.getline(a,10,'|');
                       price= atoi(a);

                        cout<<"key="<<key<<"\nname="<<pname<<"\nprice="<<price;
                                break;
                }
                else
                {
                        i++;
                        if(i%max==0)
                                i=0;
                }
        }while(i!=addr);
        if(found==0)
                cout<<"\n Record Does not exists in hash file\n";
        file.close();
        return;
}
*/