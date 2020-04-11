% Function for determining CPU cycles of C5535 Hardware FFT
function n = FFT_cycles(N)   
    arguments
       N(1,1) {mustBeMember(N, [8 16 32 64 128 256 512 1024])}
    end
    
    if N == 2^3
        n = 130;
    elseif N == 2^4
        n = 170;
    elseif N == 2^5
        n = 321;
    elseif N == 2^6
        n = 436;
    elseif N == 2^7
        n = 912;
    elseif N == 2^8
        n = 1668;
    elseif N == 2^9
        n = 3740;
    elseif N == 2^10
        n = 7315;
    else
        
    end
end
