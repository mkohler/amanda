# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

package Amanda::XferServer;
use base qw(Exporter);
use base qw(DynaLoader);
require Amanda::Xfer;
require Amanda::MainLoop;
require Amanda::Device;
require Amanda::Header;
package Amanda::XferServerc;
bootstrap Amanda::XferServer;
package Amanda::XferServer;
@EXPORT = qw();

# ---------- BASE METHODS -------------

package Amanda::XferServer;

sub TIEHASH {
    my ($classname,$obj) = @_;
    return bless $obj, $classname;
}

sub CLEAR { }

sub FIRSTKEY { }

sub NEXTKEY { }

sub FETCH {
    my ($self,$field) = @_;
    my $member_func = "swig_${field}_get";
    $self->$member_func();
}

sub STORE {
    my ($self,$field,$newval) = @_;
    my $member_func = "swig_${field}_set";
    $self->$member_func($newval);
}

sub this {
    my $ptr = shift;
    return tied(%$ptr);
}


# ------- FUNCTION WRAPPERS --------

package Amanda::XferServer;

*xfer_source_device = *Amanda::XferServerc::xfer_source_device;
*xfer_dest_device = *Amanda::XferServerc::xfer_dest_device;
*xfer_source_holding = *Amanda::XferServerc::xfer_source_holding;
*xfer_dest_taper_splitter = *Amanda::XferServerc::xfer_dest_taper_splitter;
*xfer_dest_taper_cacher = *Amanda::XferServerc::xfer_dest_taper_cacher;
*xfer_dest_taper_directtcp = *Amanda::XferServerc::xfer_dest_taper_directtcp;
*xfer_dest_taper_start_part = *Amanda::XferServerc::xfer_dest_taper_start_part;
*xfer_dest_taper_use_device = *Amanda::XferServerc::xfer_dest_taper_use_device;
*xfer_dest_taper_cache_inform = *Amanda::XferServerc::xfer_dest_taper_cache_inform;
*xfer_dest_taper_get_part_bytes_written = *Amanda::XferServerc::xfer_dest_taper_get_part_bytes_written;
*xfer_source_recovery = *Amanda::XferServerc::xfer_source_recovery;
*xfer_source_recovery_start_part = *Amanda::XferServerc::xfer_source_recovery_start_part;
*xfer_source_recovery_use_device = *Amanda::XferServerc::xfer_source_recovery_use_device;

# ------- VARIABLE STUBS --------

package Amanda::XferServer;


@EXPORT_OK = ();
%EXPORT_TAGS = ();


=head1 NAME

Amanda::XferServer - server-only parts of Amanda::Xfer

This package is automatically imported into L<Amanda::Xfer> if it
exists; it is completely documented there.

=cut



package Amanda::Xfer::Source::Device;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::XferServer::xfer_source_device(@_);
}

package Amanda::Xfer::Dest::Device;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::XferServer::xfer_dest_device(@_);
}

package Amanda::Xfer::Source::Holding;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::XferServer::xfer_source_holding(@_);
}

package Amanda::Xfer::Dest::Taper;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );
*use_device = *Amanda::XferServer::xfer_dest_taper_use_device;
*start_part = *Amanda::XferServer::xfer_dest_taper_start_part;
*cache_inform = *Amanda::XferServer::xfer_dest_taper_cache_inform;
*get_part_bytes_written = *Amanda::XferServer::xfer_dest_taper_get_part_bytes_written;

package Amanda::Xfer::Dest::Taper::Splitter;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Dest::Taper );

sub new { 
    my $pkg = shift;


    Amanda::XferServer::xfer_dest_taper_splitter(@_);
}

package Amanda::Xfer::Dest::Taper::Cacher;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Dest::Taper );

sub new { 
    my $pkg = shift;


    Amanda::XferServer::xfer_dest_taper_cacher(@_);
}

package Amanda::Xfer::Dest::Taper::DirectTCP;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Dest::Taper );

sub new { 
    my $pkg = shift;


    Amanda::XferServer::xfer_dest_taper_directtcp(@_);
}

package Amanda::Xfer::Source::Recovery;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::XferServer::xfer_source_recovery(@_);
}
*start_part = *Amanda::XferServer::xfer_source_recovery_start_part;
*use_device = *Amanda::XferServer::xfer_source_recovery_use_device;
1;