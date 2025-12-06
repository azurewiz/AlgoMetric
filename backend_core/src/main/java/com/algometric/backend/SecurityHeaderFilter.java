package com.algometric.backend;

import jakarta.servlet.*;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Component;
import java.io.IOException;

@Component
public class SecurityHeaderFilter implements Filter {

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {
        
        HttpServletResponse httpServletResponse = (HttpServletResponse) response;

        // FIX 1: Prevent MIME Sniffing (Fixes the alert in your Image 2 & 3)
        httpServletResponse.setHeader("X-Content-Type-Options", "nosniff");

        // FIX 2: Prevent Clickjacking (Fixes the 'Missing Anti-clickjacking' alert)
        httpServletResponse.setHeader("X-Frame-Options", "DENY");

        // FIX 3: Basic XSS Protection
        httpServletResponse.setHeader("X-XSS-Protection", "1; mode=block");

        // Continue the request chain so the user gets their data
        chain.doFilter(request, response);
    }
}