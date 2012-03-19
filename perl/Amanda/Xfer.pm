# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

package Amanda::Xfer;
use base qw(Exporter);
use base qw(DynaLoader);
require Amanda::MainLoop;
package Amanda::Xferc;
bootstrap Amanda::Xfer;
package Amanda::Xfer;
@EXPORT = qw();

# ---------- BASE METHODS -------------

package Amanda::Xfer;

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

package Amanda::Xfer;

*xfer_new = *Amanda::Xferc::xfer_new;
*xfer_unref = *Amanda::Xferc::xfer_unref;
*xfer_get_status = *Amanda::Xferc::xfer_get_status;
*xfer_repr = *Amanda::Xferc::xfer_repr;
*xfer_start = *Amanda::Xferc::xfer_start;
*xfer_cancel = *Amanda::Xferc::xfer_cancel;
*xfer_element_unref = *Amanda::Xferc::xfer_element_unref;
*xfer_element_repr = *Amanda::Xferc::xfer_element_repr;
*same_elements = *Amanda::Xferc::same_elements;
*xfer_source_random = *Amanda::Xferc::xfer_source_random;
*xfer_source_random_get_seed = *Amanda::Xferc::xfer_source_random_get_seed;
*xfer_source_pattern = *Amanda::Xferc::xfer_source_pattern;
*xfer_source_fd = *Amanda::Xferc::xfer_source_fd;
*xfer_source_directtcp_listen = *Amanda::Xferc::xfer_source_directtcp_listen;
*xfer_source_directtcp_listen_get_addrs = *Amanda::Xferc::xfer_source_directtcp_listen_get_addrs;
*xfer_source_directtcp_connect = *Amanda::Xferc::xfer_source_directtcp_connect;
*xfer_filter_xor = *Amanda::Xferc::xfer_filter_xor;
*xfer_filter_process = *Amanda::Xferc::xfer_filter_process;
*get_err_fd = *Amanda::Xferc::get_err_fd;
*xfer_dest_null = *Amanda::Xferc::xfer_dest_null;
*xfer_dest_buffer = *Amanda::Xferc::xfer_dest_buffer;
*xfer_dest_buffer_get = *Amanda::Xferc::xfer_dest_buffer_get;
*xfer_dest_fd = *Amanda::Xferc::xfer_dest_fd;
*xfer_dest_directtcp_listen = *Amanda::Xferc::xfer_dest_directtcp_listen;
*xfer_dest_directtcp_listen_get_addrs = *Amanda::Xferc::xfer_dest_directtcp_listen_get_addrs;
*xfer_dest_directtcp_connect = *Amanda::Xferc::xfer_dest_directtcp_connect;
*xfer_get_amglue_source = *Amanda::Xferc::xfer_get_amglue_source;

# ------- VARIABLE STUBS --------

package Amanda::Xfer;

*XFER_INIT = *Amanda::Xferc::XFER_INIT;
*XFER_START = *Amanda::Xferc::XFER_START;
*XFER_RUNNING = *Amanda::Xferc::XFER_RUNNING;
*XFER_DONE = *Amanda::Xferc::XFER_DONE;
*XMSG_INFO = *Amanda::Xferc::XMSG_INFO;
*XMSG_ERROR = *Amanda::Xferc::XMSG_ERROR;
*XMSG_DONE = *Amanda::Xferc::XMSG_DONE;
*XMSG_CANCEL = *Amanda::Xferc::XMSG_CANCEL;
*XMSG_PART_DONE = *Amanda::Xferc::XMSG_PART_DONE;
*XMSG_READY = *Amanda::Xferc::XMSG_READY;

@EXPORT_OK = ();
%EXPORT_TAGS = ();


=head1 NAME

Amanda::Xfer - the transfer architecture

=head1 SYNOPSIS

  use Amanda::MainLoop;
  use Amanda::Xfer qw( :constants );
  use POSIX;

  my $infd = POSIX::open("input", POSIX::O_RDONLY, 0);
  my $outfd = POSIX::open("output", POSIX::O_CREAT|POSIX::O_WRONLY, 0640);
  my $xfer = Amanda::Xfer->new([
    Amanda::Xfer::Source::Fd->new($infd),
    Amanda::Xfer::Dest::Fd->new($outfd)
  ]);
  $xfer->start(sub {
      my ($src, $xmsg, $xfer) = @_;
      print "Message from $xfer: $xmsg\n"; # use stringify operations
      if ($msg->{'type'} == $XMSG_DONE) {
	  Amanda::MainLoop::quit();
      }
  }, 0, 0);
  Amanda::MainLoop::run();

See L<http://wiki.zmanda.com/index.php/XFA> for background on the
transfer architecture.

=head1 Amanda::Xfer Objects

A new transfer is created with C<< Amanda::Xfer->new() >>, which takes
an arrayref giving the transfer elements which should compose the
transfer.

The resulting object has the following methods:

=over

=item start($cb, $offset, $size)

Start this transfer.  It transfer $size bytes starting from offset $offset.
$offset must be 0. $size is only supported by Amanda::Xfer::Source::Recovery.
A size of 0 transfer everything to EOF.
Processing takes place asynchronously, and messages will
begin queueing up immediately.  If C<$cb> is given, then it is installed as the
callback for messages from this transfer.  The callback receives three
arguments: the event source, the message, and a reference to the controlling
transfer.  See the description of C<Amanda::Xfer::Msg>, below, for details.

There is no need to remove the source on completion of the transfer - that is
handled for you.

=item cancel()

Stop transferring data.  The transfer will send an C<XMSG_CANCEL>,
"drain" any buffered data as best it can, and then complete normally
with an C<XMSG_DONE>.

=item get_status()

Get the transfer's status.  The result will be one of C<$XFER_INIT>,
C<$XFER_START>, C<$XFER_RUNNING>, or C<$XFER_DONE>.  These symbols are
available for import with the tag C<:constants>.

=item repr()

Return a string representation of this transfer, suitable for use in
debugging messages.  This method is automatically invoked when a
transfer is interpolated into a string:

  print "Starting $xfer\n";

=item get_source()

Get the L<Amanda::MainLoop> event source through which messages will
be delivered for this transfer.  Use its C<set_callback> method to
connect a perl sub for processing events. 

Use of this method is deprecated; instead, pass a callback to the C<start>
method.  If you set a callback via C<get_source>, then you I<must> C<remove>
the source when the transfer is complete!

=back

=head1 Amanda::Xfer::Element objects

The individual transfer elements that compose a transfer are instances
of subclasses of Amanda::Xfer::Element.  All such objects have a
C<repr()> method, similar to that for transfers, and support a similar
kind of string interpolation.

Note that the names of these classes contain the words "Source",
"Filter", and "Dest".  This is merely suggestive of their intended
purpose -- there are no such abstract classes.

=head2 Transfer Sources

=head3 Amanda::Xfer::Source::Device (SERVER ONLY)

  Amanda::Xfer::Source::Device->new($device);

This source reads data from a device.  The device should already be
queued up for reading (C<< $device->seek_file(..) >>).  The element
will read until the end of the device file.

=head3 Amanda::Xfer::Source::Fd

  Amanda::Xfer::Source::Fd->new(fileno($fh));

This source reads data from a file descriptor.  It reads until EOF,
but does not close the descriptor.  Be careful not to let Perl close
the file for you!

=head3 Amanda::Xfer::Source::Holding (SERVER-ONLY)

  Amanda::Xfer::Source::Holding->new($filename);

This source reads data from a holding file (see L<Amanda::Holding>).
If the transfer only consists of a C<Amanda::Xfer::Source::Holding>
and an C<Amanda::Xfer::Dest::Taper::Cacher> (with no filters), then the source
will call the destination's C<cache_inform> method so that it can use
holding chunks for a split-part cache.

=head3 Amanda::Xfer::Source::Random

  Amanda::Xfer::Source::Random->new($length, $seed);

This source provides I<length> bytes of random data (or an unlimited
amount of data if I<length> is zero).  C<$seed> is the seed used to
generate the random numbers; this seed can be used in a destination to
check for correct output.

If you need to string multiple transfers together into a coherent sequence of
random numbers, for example when testing the re-assembly of spanned dumps, call

  my $seed = $src->get_seed();

to get the finishing seed for the source, then pass this to the source
constructor for the next transfer.  When concatenated, the bytestreams from the
transfers will verify correctly using the original random seed.

=head3 Amanda::Xfer::Source::Pattern

  Amanda::Xfer::Source::Pattern->new($length, $pattern);

This source provides I<length> bytes containing copies of
I<pattern>. If I<length> is zero, the source provides an unlimited
number of bytes.

=head3 Amanda::Xfer::Source::Recovery (SERVER ONLY)

  Amanda::Xfer::Source::Recovery->new($first_device);

This source reads a datastream composed of on-device files.  Its constructor
takes a pointer to the first device that will be read from; this is used
internally to determine whether DirectTCP is supported.

The element sense C<$XMSG_READY> when it is ready for the first C<start_part>
invocation.  Don't do anything with the device between the start of the
transfer and when the element sends an C<$XMSG_READY>.

The element contains no logic to decide I<which> files to assemble into the
datastream; instead, it relies on the caller to supply pre-positioned devices:

  $src->start_part($device);

Once C<start_part> is called, the source will read until C<$device> produces an
EOF.  As each part is completed, the element sends an C<$XMSG_PART_DONE>
L<Amanda::Xfer::Msg>, with the following keys:

 size	    bytes read from the device
 duration   time spent reading
 fileno     the on-media file number from which the part was read

Call C<start_part> with C<$device = undef> to indicate that there are no more
parts.

To switch to a new device in mid-transfer, use C<use_device>:

  $dest->use_device($device);

This method must be called with a device that is not yet started, and thus must
be called before the C<start_part> method is called with a new device.

=head3 Amanda::Xfer::Source::DirectTCPListen

  Amanda::Xfer::Source::DirectTCPListen->new();

This source is for use when the transfer data will come in via DirectTCP, with
the data's I<source> connecting to the data's I<destination>.  That is, the
data source is the connection initiator.  Set up the transfer, and after
starting it, call this element's C<get_addrs> method to get an arrayref of ip/port pairs,
e.g., C<[ "192.168.4.5", 9924 ]>, all of which are listening for an incoming
data connection.  Once a connection arrives, this element will read data from
it and send those data into the transfer.

  my $addrs = $src->get_addrs();

=head3 Amanda::Xfer::Source::DirectTCPConnect

  Amanda::Xfer::Source::DirectTCPConnect->new($addrs);

This source is for use when the transfer data will come in via DirectTCP, with
the data's I<destination> connecting to the the data's I<source>.  That is, the
data destination is the connection initiator.  The element connects to
C<$addrs> and reads the transfer data from the connection.

=head2 Transfer Filters

=head3 Amanda::Xfer::Filter:Process

  $xfp = Amanda::Xfer::Filter::Process->new([@args], $need_root);

This filter will pipe data through the standard file descriptors of the
subprocess specified by C<@args>.  If C<$need_root> is true, it will attempt to
change to uid 0 before executing the process.  Note that the process is
invoked directly, not via a shell, so shell metacharcters (e.g., C<< 2>&1 >>)
will not function as expected. This method create a pipe for the process
stderr and the caller must read it or a hang may occur.

  $xfp->get_stderr_fd()

Return the file descriptor of the stderr pipe to read from.

=head3 Amanda::Xfer::Filter:Xor

  Amanda::Xfer::Filter::Xor->new($key);

This filter applies a bytewise XOR operation to the data flowing
through it.

=head2 Transfer Destinations

=head3 Amanda::Xfer::Dest::Device (SERVER ONLY)

  Amanda::Xfer::Dest::Device->new($device, $cancel_at_eom);

This source writes data to a device.  The device should be ready for writing
(C<< $device->start_file(..) >>).  On completion of the transfer, the file will
be finished.  If an error occurs, or if C<$cancel_at_eom> is true and the
device signals LEOM, the transfer will be cancelled.

Note that this element does not apply any sort of stream buffering.

=head3 Amanda::Xfer::Dest::Buffer

  Amanda::Xfer::Dest::Buffer->new($max_size);

This destination records data into an in-memory buffer which can grow up to
C<$max_size> bytes.  The buffer is available with the C<get> method, which
returns a copy of the buffer as a perl scalar:

    my $buf = $xdb->get();

=head3 Amanda::Xfer::Dest::DirectTCPListen

  Amanda::Xfer::Dest::DirectTCPListen->new();

This destination is for use when the transfer data will come in via DirectTCP,
with the data's I<destination> connecting to the data's I<source>.  That is,
the data destination is the connection initiator.  Set up the transfer, and
after starting it, call this element's C<get_addrs> method to get an arrayref
of ip/port pairs, e.g., C<[ "192.168.4.5", 9924 ]>, all of which are listening
for an incoming data connection.  Once a connection arrives, this element will
write the transfer data to it.

  my $addrs = $src->get_addrs();

=head3 Amanda::Xfer::Dest::DirectTCPConnect

  Amanda::Xfer::Dest::DirectTCPConnect->new($addrs);

This destination is for use when the transfer data will come in via DirectTCP,
with the data's I<source> connecting to the the data's I<destination>.  That
is, the data source is the connection initiator.  The element connects to
C<$addrs> and writes the transfer data to the connection.

=head3 Amanda::Xfer::Dest::Fd

  Amanda::Xfer::Dest::Fd->new(fileno($fh));

This destination writes data to a file descriptor.  The file is not
closed after the transfer is completed.  Be careful not to let Perl
close the file for you!

=head3 Amanda::Xfer::Dest::Null

  Amanda::Xfer::Dest::Null->new($seed);

This destination discards the data it receives.  If C<$seed> is
nonzero, then the element will validate that it receives the data that
C<Amanda::Xfer::Source::Random> produced with the same seed.  No
validation is performed if C<$seed> is zero.

=head3 Amanda::Xfer::Dest::Taper (SERVER ONLY)

This is the parent class to C<Amanda::Xfer::Dest::Taper::Cacher> and
C<Amanda::Xfer::Dest::Taper::DirectTCP>. These subclasses allow a single
transfer to write to multiple files (parts) on a device, and even spread those
parts over multiple devices, without interrupting the transfer itself.

The subclass constructors all take a C<$first_device>, which should be
configured but not yet started; and a C<$part_size> giving the maximum size of
each part.  Note that this value may be rounded up internally as necessary.

When a transfer using a taper destination element is first started, no data is
transfered until the element's C<start_part> method is called:

  $dest->start_part($retry_part);

where C<$device> is the device to which the part should be written.  The device
should have a file open and ready to write (that is, 
C<< $device->start_file(..) >> has already been called).  If C<$retry_part> is
true, then the previous, unsuccessful part will be retried.

As each part is completed, the element sends an C<$XMSG_PART_DONE>
C<Amanda::Xfer::Msg>, with the following keys:

 successful true if the part was written successfully
 eof	    recipient should not call start_part again
 eom        this volume is at EOM; a new volume is required
 size	    bytes written to volume
 duration   time spent writing, not counting changer ops, etc.
 partnum    the zero-based number of this part in the overall dumpfile
 fileno     the on-media file number used for this part, or 0 if no file
            was used

If C<eom> is true, then the caller should find a new volume before
continuing.  If C<eof> is not true, then C<start_part> should be called
again, with C<$retry_part = !successful>.  Note that it is possible
for some destinations to write a portion of a part successfully,
but still stop at EOM.  That is, C<eom> does not necessarily imply
C<!successful>.

To switch to a new device in mid-transfer, use C<use_device>:

  $dest->use_device($device);

This method must be called with a device that is not yet started.

If neither the memory nor disk caches are in use, but the dumpfile is
available on disk, then the C<cache_inform> method allows the element
to use that on-disk data to support retries.  This is intended to
support transfers from Amanda's holding disk (see
C<Amanda::Xfer::Source::Holding>), but may be useful for other
purposes.

  $dest->cache_inform($filename, $offset, $length);

This function indicates that C<$filename> contains C<$length> bytes of
data, beginning at offset C<$offset> from the beginning of the file.
These bytes are assumed to follow immediately after any bytes
previously specified to C<cache_inform>.  That is, no gaps or overlaps
are allowed in the data stream described to C<cache_inform>.
Furthermore, the location of each byte must be specified to this
method I<before> it is sent through the transfer.

  $dest->get_part_bytes_written();

This function returns the number of bytes written for the current part
to the device.

=head3 Amanda::Xfer::Dest::Taper::Splitter

  Amanda::Xfer::Dest::Taper::Splitter->new($first_device, $max_memory,
                        $part_size, $expect_cache_inform);

This class splits a data stream into parts on the storage media.  It is for use
when the device supports LEOM, when the dump is already available on disk
(C<cache_inform>), or when no caching is desired.  It does not cache parts, so
it can only retry a partial part if the transfer source is calling
C<cache_inform>.  If the element is used with devices that do not support LEOM,
then it will cancel the entire transfer if the device reaches EOM and
C<cache_inform> is not in use.  Set C<$expect_cache_inform> appropriately based
on the incoming data.

The C<$part_size> and C<$first_device> parameters are described above for
C<Amanda::Xfer::Dest::Taper>.

=head3 Amanda::Xfer::Dest::Taper::Cacher

  Amanda::Xfer::Dest::Taper::Cacher->new($first_device, $max_memory,
                        $part_size, $use_mem_cache, $disk_cache_dirname);

This class is similar to the splitter, but caches data from each part in one of
a variety of ways to support "rewinding" to retry a failed part (e.g., one that
does not fit on a device).  It assumes that when a device reaches EOM while
writing, the entire on-volume file is corrupt - that is, that the device does
not support logical EOM.  The class does not support C<cache_inform>.

The C<$part_size> and C<$first_device> parameters are described above for
C<Amanda::Xfer::Dest::Taper>.

If C<$use_mem_cache> is true, each part will be cached in memory (using
C<$part_size> bytes of memory; plan accordingly!).  If C<$disk_cache_dirname>
is defined, then each part will be cached on-disk in a file in this directory.
It is an error to specify both in-memory and on-disk caching.  If neither
option is specified, the element will operate successfully, but will not be
able to retry a part, and will cancel the transfer if a part fails.

=head3 Amanda::Xfer::Dest::Taper::DirectTCP

  Amanda::Xfer::Dest::Taper::DirectTCP->new($first_device, $part_size);

This class uses the Device API DirectTCP methods to write data to a device via
DirectTCP.  Since all DirectTCP devices support logical EOM, this class does
not cache any data, and will never re-start an unsuccessful part.

As state above, C<$first_device> must not be started when C<new> is called.
Furthermore, no use of that device is allowed until the element sens an
C<$XMSG_READY> to indicate that it is finished with the device.  The
C<start_part> method must not be called until this method is received either.

=head1 Amanda::Xfer::Msg objects

Messages are simple hashrefs, with a few convenience methods.  Like
transfers, they have a C<repr()> method that formats the message
nicely, and is available through string interpolation:

  print "Received message $msg\n";

The canonical description of the message types and keys is in
C<xfer-src/xmsg.h>, and is not duplicated here.  Every message has the
following basic keys.

=over

=item type

The message type -- one of the C<xmsg_type> constants available from
the import tag C<:constants>.

=item elt

The transfer element that sent the message.

=item version

The version of the message.  This is used to support extensibility of
the protocol.

=back

Additional keys are described in the documentation for the elements
that use them.  All keys are listed in C<xfer-src/xmsg.h>.

=cut



push @EXPORT_OK, qw(xfer_status_to_string);
push @{$EXPORT_TAGS{"xfer_status"}}, qw(xfer_status_to_string);

my %_xfer_status_VALUES;
#Convert an enum value to a single string
sub xfer_status_to_string {
    my ($enumval) = @_;

    for my $k (keys %_xfer_status_VALUES) {
	my $v = $_xfer_status_VALUES{$k};

	#is this a matching flag?
	if ($enumval == $v) {
	    return $k;
	}
    }

#default, just return the number
    return $enumval;
}

push @EXPORT_OK, qw($XFER_INIT);
push @{$EXPORT_TAGS{"xfer_status"}}, qw($XFER_INIT);

$_xfer_status_VALUES{"XFER_INIT"} = $XFER_INIT;

push @EXPORT_OK, qw($XFER_START);
push @{$EXPORT_TAGS{"xfer_status"}}, qw($XFER_START);

$_xfer_status_VALUES{"XFER_START"} = $XFER_START;

push @EXPORT_OK, qw($XFER_RUNNING);
push @{$EXPORT_TAGS{"xfer_status"}}, qw($XFER_RUNNING);

$_xfer_status_VALUES{"XFER_RUNNING"} = $XFER_RUNNING;

push @EXPORT_OK, qw($XFER_DONE);
push @{$EXPORT_TAGS{"xfer_status"}}, qw($XFER_DONE);

$_xfer_status_VALUES{"XFER_DONE"} = $XFER_DONE;

#copy symbols in xfer_status to constants
push @{$EXPORT_TAGS{"constants"}},  @{$EXPORT_TAGS{"xfer_status"}};

push @EXPORT_OK, qw(xmsg_type_to_string);
push @{$EXPORT_TAGS{"xmsg_type"}}, qw(xmsg_type_to_string);

my %_xmsg_type_VALUES;
#Convert an enum value to a single string
sub xmsg_type_to_string {
    my ($enumval) = @_;

    for my $k (keys %_xmsg_type_VALUES) {
	my $v = $_xmsg_type_VALUES{$k};

	#is this a matching flag?
	if ($enumval == $v) {
	    return $k;
	}
    }

#default, just return the number
    return $enumval;
}

push @EXPORT_OK, qw($XMSG_INFO);
push @{$EXPORT_TAGS{"xmsg_type"}}, qw($XMSG_INFO);

$_xmsg_type_VALUES{"XMSG_INFO"} = $XMSG_INFO;

push @EXPORT_OK, qw($XMSG_ERROR);
push @{$EXPORT_TAGS{"xmsg_type"}}, qw($XMSG_ERROR);

$_xmsg_type_VALUES{"XMSG_ERROR"} = $XMSG_ERROR;

push @EXPORT_OK, qw($XMSG_DONE);
push @{$EXPORT_TAGS{"xmsg_type"}}, qw($XMSG_DONE);

$_xmsg_type_VALUES{"XMSG_DONE"} = $XMSG_DONE;

push @EXPORT_OK, qw($XMSG_CANCEL);
push @{$EXPORT_TAGS{"xmsg_type"}}, qw($XMSG_CANCEL);

$_xmsg_type_VALUES{"XMSG_CANCEL"} = $XMSG_CANCEL;

push @EXPORT_OK, qw($XMSG_PART_DONE);
push @{$EXPORT_TAGS{"xmsg_type"}}, qw($XMSG_PART_DONE);

$_xmsg_type_VALUES{"XMSG_PART_DONE"} = $XMSG_PART_DONE;

push @EXPORT_OK, qw($XMSG_READY);
push @{$EXPORT_TAGS{"xmsg_type"}}, qw($XMSG_READY);

$_xmsg_type_VALUES{"XMSG_READY"} = $XMSG_READY;

#copy symbols in xmsg_type to constants
push @{$EXPORT_TAGS{"constants"}},  @{$EXPORT_TAGS{"xmsg_type"}};

sub xfer_start_with_callback {
    my ($xfer, $cb, $offset, $size) = @_;
    if (defined $cb) {
	my $releasing_cb = sub {
	    my ($src, $msg, $xfer) = @_;
	    my $done = $msg->{'type'} == $XMSG_DONE;
	    $src->remove() if $done;
	    $cb->(@_);
	    $cb = undef if $done; # break potential reference loop
	};
	$xfer->get_source()->set_callback($releasing_cb);
    }
    $offset = 0 if !defined $offset;
    $size = 0 if !defined $size;
    xfer_start($xfer, $offset, $size);
}

sub xfer_set_callback {
    my ($xfer, $cb) = @_;
    if (defined $cb) {
	my $releasing_cb = sub {
	    my ($src, $msg, $xfer) = @_;
	    my $done = $msg->{'type'} == $XMSG_DONE;
	    $src->remove() if $done;
	    $cb->(@_);
	    $cb = undef if $done; # break potential reference loop
       };
	$xfer->get_source()->set_callback($releasing_cb);
    } else {
	$xfer->get_source()->set_callback(undef);
    }
}

package Amanda::Xfer::Xfer;

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_new(@_);
}
*DESTROY = *Amanda::Xfer::xfer_unref;

use overload '""' => sub { $_[0]->repr(); };

use overload '==' => sub {     Amanda::Xfer::same_elements($_[0], $_[1]); };
use overload '!=' => sub { not Amanda::Xfer::same_elements($_[0], $_[1]); };
*repr = *Amanda::Xfer::xfer_repr;
*get_status = *Amanda::Xfer::xfer_get_status;
*get_source = *Amanda::Xfer::xfer_get_amglue_source;
*start = *Amanda::Xfer::xfer_start_with_callback;
*set_callback = *Amanda::Xfer::xfer_set_callback;
*cancel = *Amanda::Xfer::xfer_cancel;

package Amanda::Xfer::Element;
*DESTROY = *Amanda::Xfer::xfer_element_unref;

use overload '""' => sub { $_[0]->repr(); };

use overload '==' => sub {     Amanda::Xfer::same_elements($_[0], $_[1]); };
use overload '!=' => sub { not Amanda::Xfer::same_elements($_[0], $_[1]); };
*repr = *Amanda::Xfer::xfer_element_repr;

package Amanda::Xfer::Element::Glue;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

package Amanda::Xfer::Source::Fd;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_source_fd(@_);
}

package Amanda::Xfer::Source::Random;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_source_random(@_);
}
*get_seed = *Amanda::Xfer::xfer_source_random_get_seed;

package Amanda::Xfer::Source::DirectTCPListen;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_source_directtcp_listen(@_);
}
*get_addrs = *Amanda::Xfer::xfer_source_directtcp_listen_get_addrs;

package Amanda::Xfer::Source::DirectTCPConnect;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_source_directtcp_connect(@_);
}

package Amanda::Xfer::Source::Pattern;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_source_pattern(@_);
}

package Amanda::Xfer::Filter::Xor;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_filter_xor(@_);
}

package Amanda::Xfer::Filter::Process;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_filter_process(@_);
}
*get_stderr_fd = *Amanda::Xfer::get_err_fd;

package Amanda::Xfer::Dest::Fd;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_dest_fd(@_);
}

package Amanda::Xfer::Dest::Null;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_dest_null(@_);
}

package Amanda::Xfer::Dest::Buffer;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_dest_buffer(@_);
}
*get = *Amanda::Xfer::xfer_dest_buffer_get;

package Amanda::Xfer::Dest::DirectTCPListen;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_dest_directtcp_listen(@_);
}
*get_addrs = *Amanda::Xfer::xfer_dest_directtcp_listen_get_addrs;

package Amanda::Xfer::Dest::DirectTCPConnect;

use vars qw(@ISA);
@ISA = qw( Amanda::Xfer::Element );

sub new { 
    my $pkg = shift;


    Amanda::Xfer::xfer_dest_directtcp_connect(@_);
}

package Amanda::Xfer::Msg;

use Data::Dumper;
use overload '""' => sub { $_[0]->repr(); };

sub repr {
    my ($self) = @_;
    local $Data::Dumper::Indent = 0;
    local $Data::Dumper::Terse = 1;
    local $Data::Dumper::Useqq = 1;

    my $typestr = Amanda::Xfer::xmsg_type_to_string($self->{'type'});
    my $str = "{ type => \$$typestr, elt => $self->{'elt'}, version => $self->{'version'},";

    my %skip = ( "type" => 1, "elt" => 1, "version" => 1 );
    for my $k (keys %$self) {
	next if $skip{$k};
	$str .= " $k => " . Dumper($self->{$k}) . ",";
    }

    # strip the trailing comma and add a closing brace
    $str =~ s/,$/ }/g;

    return $str;
}

package Amanda::Xfer;

# make Amanda::Xfer->new equivalent to Amanda::Xfer::Xfer->new (don't
# worry, the blessings work out just fine)
*new = *Amanda::Xfer::Xfer::new;

# try to load Amanda::XferServer, which is server-only.  If it's not found, then
# its classes just remain undefined.
BEGIN {
    use Amanda::Util;
    if (Amanda::Util::built_with_component("server")) {
	eval "use Amanda::XferServer;";
    }
}
1;
